﻿#pragma once
/**
 * @file NGraph.h
 * @brief グラフ実装
 * @author takiren
 *
 */

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <thread>

#include "NUtils.h"
#include "NVariant.h"
#include "json.hpp"

namespace noveil {

using json = nlohmann::json;
// グラフ
class NGraphBase;
class NSequencer;

// 頂点
class NNodeBase;
class NNodeAuxiliary;
class NNodeExecutor;

// イベント
class NEventBase;
class NEventImpl;

// ピン
class NNodePinBase;
class NNodePinExecution; /*!いらないかも*/

// T
class NNodeTemplate;

/*
 * 弱参照
 */
template <typename T>
using ParentRef = std::weak_ptr<T>;

template <typename T>
using PackedParentRef = std::vector<ParentRef<T>>;

/*
 * 強参照
 */
template <typename T>
using ChildRef = std::shared_ptr<T>;

template <typename T>
using PackedChildRef = std::vector<ChildRef<T>>;

/*!次に実行されるノードの弱参照*/
using NNodeExecutorWeakRef = std::weak_ptr<NNodeExecutor>;

/*
 * ピンのファンクタの型
 * See also NNodePinBase::GetData()
 */

using CallbackFunction = std::function<Variant(void)>;

/*!NNodeの情報を受け渡す。*/
class NNodeDescriptor;

class NExecutionImpl;

class NExecutionImpl {
 public:
  virtual void operator()(PackedParentRef<NNodePinBase>,
                          PackedParentRef<NNodePinBase>, PackedVariant){};
};

/**グラフの頂点となる*/
class NNodeBase : public INInfo, private Noncopyable {
 private:
  std::function<void(PackedParentRef<NNodePinBase>,
                     PackedParentRef<NNodePinBase>)>
      executor;

  ParentRef<NNodeBase> nextNode;

 protected:
  PackedChildRef<NNodePinBase> pinInput;
  PackedChildRef<NNodePinBase> pinOutput;

  std::function<void()> executionCall;

 public:
  explicit NNodeBase() : INInfo(){};
  virtual ~NNodeBase() = default;

  inline void AddInputNode(NNodePinBase* inPin) {
    pinInput.emplace_back(std::shared_ptr<NNodePinBase>(inPin));
  };

  inline void AddOutputNode(NNodePinBase* outPin) {
    pinOutput.emplace_back(std::shared_ptr<NNodePinBase>(outPin));
  };

  inline PackedParentRef<NNodePinBase> GetPinInputRef() {
    PackedParentRef<NNodePinBase> ret;
    for (auto it = pinInput.begin(); it != pinInput.end(); ++it) {
      ParentRef<NNodePinBase> p = *it;
      ret.emplace_back(p);
    }
    return ret;
  };

  inline PackedParentRef<NNodePinBase> GetPinOutputRef() {
    PackedParentRef<NNodePinBase> ret;
    for (auto it = pinOutput.begin(); it != pinOutput.end(); ++it) {
      ParentRef<NNodePinBase> p = *it;
      ret.emplace_back(p);
    }
    return ret;
  };

  /*!ピンの数*/
  inline int GetPinInputSize() const { return pinInput.size(); }
  inline int GetPinOutputSize() const { return pinOutput.size(); }

  inline void BindToNextNode(
      ParentRef<NNodeBase> node) {
    this->nextNode = node;
  }

  template <class T>
  void SetImplementation(const T&& impl) {
    this->executor = impl;
  }

  template <class... Args>
  inline void Execute(Args... args) {
    this->executor(GetPinInputRef(), GetPinOutputRef(), args...);
    if (!nextNode.expired()) this->nextNode.lock()->Execute();
  };

  json GetJson(){};
};

/**多分いらない*/
class NNodeTemplate : public NNodeBase {
 public:
  explicit NNodeTemplate() : NNodeBase(){};
  virtual ~NNodeTemplate() = default;
};

/**
 *ノードを管理するための基底クラス。
 *
 */

class NGraphBase : public INInfo, private Noncopyable {
 private:
 protected:
  hash_map<NUUID, ChildRef<NNodeBase>> nodes;

 public:
  NGraphBase() = default;
  virtual ~NGraphBase() = default;

  bool Connect(NUUID& from, NUUID& to){};
  bool Connect(ParentRef<NNodeBase> from, ParentRef<NNodeBase> ref) {}
};

class NGraphTemplate : public NGraphBase {
 public:
  explicit NGraphTemplate() = default;
  virtual ~NGraphTemplate() = default;
};

class NEventBase {
 public:
  explicit NEventBase() = default;
  virtual ~NEventBase() = default;
  void DoAction(){};
};

/**
 * ノードの値入出力に必要なオブジェクト:=ピンの基底クラス。
 */
class NNodePinBase : public INInfo, private Noncopyable {
 private:
  /** std::variantのラッパークラス*/
  Variant mValue;

  // TODO: Rename the valiable name to better one.
  /*!これに受け取りたいデータを返す関数オブジェクトを登録*/
  CallbackFunction Get_val_call;

  /**
   * ピンを保持するNNodeBaseへの参照エイリアス
   */
  using ParentNodeRef = ParentRef<NNodeBase>;

  /*!ピンを保持するNNodeBaseへの参照*/
  ParentNodeRef parentNode;

 protected:
  std::vector<std::string> allowedList;

 public:
  explicit NNodePinBase() : INInfo(){};
  virtual ~NNodePinBase() = default;

  NNodePinBase(NNodePinBase&&) = default;

  /*
   * Binds a functor which type is std::function<Variant(void)>.
   * It holds copy.
   * 与えられた関数オブジェクトのコピーを持つ。
   * @warning BindWithRef, BindWithForwardingも
   */
  inline void Bind(const CallbackFunction rhs) { Get_val_call = rhs; };

  inline void BindWithRef(const CallbackFunction& rhs) {
    Get_val_call = std::ref(rhs);
  }

  inline void BindWithForwarding(const CallbackFunction&& rhs) {
    Get_val_call = std::forward<decltype(rhs)>(rhs);
  };

  /*!Binds like std::bind(&Hoge::fuga,std::ref(pointer))*/
  template <class T, class F>
  inline void Bind(const T& a, const F& boundObject) {
    Get_val_call = std::bind(a, std::ref(boundObject));
  };

  inline std::optional<Variant> value() {
    assert(Get_val_call);
    return Get_val_call();
  }
};

/*
 * 実行ノードクラス
 * @note 何らかの処理を行うクラス。
 */
class NNodeExecutor : public NNodeBase {
 protected:
  NNodeExecutorWeakRef parent;

 public:
  NNodeExecutor() = default;
  virtual ~NNodeExecutor() = default;
};

/*
 * NNodeExecutorに必要なVariantを提供するクラス。
 * NNodePinを保持する。
 */
class NNodeAuxiliary : public NNodeBase {
 protected:
 public:
  NNodeAuxiliary() = default;
  virtual ~NNodeAuxiliary() = default;
};

class NEventImpl : public NEventBase {
 private:
 protected:
 public:
  explicit NEventImpl() = default;
  virtual ~NEventImpl() = default;
};

class NNodeEntryPoint : public INInfo, private Noncopyable {
 private:
  ParentRef<NNodeBase> nextNode;

 public:
  explicit NNodeEntryPoint() = default;
  virtual ~NNodeEntryPoint() = default;
  void SetNextNode(ParentRef<NNodeBase> node) { nextNode = node; };
  void Start() { nextNode.lock()->Execute(); };
};

class NNodeEndPoint : public INInfo, private Noncopyable {
 private:
 public:
};

class NSequencer : public NGraphBase {
 private:
  ChildRef<NNodeEntryPoint> entryPoint;
  ChildRef<NNodeEndPoint> endPoint;

 protected:
 public:
  explicit NSequencer()
      : entryPoint(std::make_shared<NNodeEntryPoint>()),
        endPoint(std::make_shared<NNodeEndPoint>()){};
  virtual ~NSequencer() = default;

  void operator()() { entryPoint->Start(); };

  void Execute() { entryPoint->Start(); }

  ParentRef<NNodeBase> AddNode() {
    auto* node = new NNodeBase();

    auto ret = nodes.emplace(node->GetUID(), std::shared_ptr<NNodeBase>(node));
    // 最初のノードの場合エントリポイントをつなぐ。
    if (nodes.size() == 1) {
      entryPoint->SetNextNode(ret.first->second);
    }
    return ret.first->second;
  }
};

/*!NNodeが持つ値を格納する。*/
class NNodeDescriptor {
  json j;
  NNodeDescriptor(const NNodeBase& rhs) {
    j["name"] = rhs.GetName();
    j["uid"] = rhs.GetUID().bytes();
  }
};

}  // namespace noveil