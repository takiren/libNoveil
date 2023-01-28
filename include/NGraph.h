#pragma once
/**
 * @file NGraph.h
 * @brief グラフ実装
 * @author takiren
 *
 */

#include <algorithm>
#include <functional>
#include <memory>
#include <queue>

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
class NNodePinInput;
class NNodePinOutput;
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

class NExecutionImpl {};

/**グラフの頂点となる*/
class NNodeBase : public INInfo, private Noncopyable {
 private:
  NExecutionImpl executor;

 protected:
  PackedChildRef<NNodePinInput> pinInput;
  PackedChildRef<NNodePinOutput> pinOutput;

  std::function<void()> executionCall;

 public:
  explicit NNodeBase() : INInfo(){};
  virtual ~NNodeBase() = default;

  void AddInputNode(NNodePinInput* inPin) {
    pinInput.emplace_back(std::shared_ptr<NNodePinInput>(inPin));
  };

  void AddOutputNode(NNodePinOutput* outPin) {
    pinOutput.emplace_back(std::shared_ptr<NNodePinOutput>(outPin));
  };

  inline PackedParentRef<NNodePinInput> GetPinInputRef() {
    PackedParentRef<NNodePinInput> ret;
    for (auto it = pinInput.begin(); it != pinInput.end(); ++it) {
      ParentRef<NNodePinInput> p = *it;
      ret.emplace_back(p);
    }
    return ret;
  };

  inline PackedParentRef<NNodePinOutput> GetPinOutputRef() {
    PackedParentRef<NNodePinOutput> ret;
    for (auto it = pinOutput.begin(); it != pinOutput.end(); ++it) {
      ParentRef<NNodePinOutput> p = *it;
      ret.emplace_back(p);
    }
    return ret;
  };

  inline int GetPinInputSize() const { return pinInput.size(); }
  inline int GetPinOutputSize() const { return pinOutput.size(); }

  virtual void Execute(){};

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
 public:
  NGraphBase() = default;
  virtual ~NGraphBase() = default;
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
   * ピンを保持するNNodeBaseへの参照
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
  void Bind(const CallbackFunction rhs) { Get_val_call = rhs; };

  void BindWithRef(const CallbackFunction& rhs) {
    Get_val_call = std::ref(rhs);
  }

  void BindWithForwarding(const CallbackFunction&& rhs) {
    Get_val_call = std::forward<decltype(rhs)>(rhs);
  };

  /*!Binds like std::bind(&Hoge::fuga,std::ref(pointer))*/
  template <class T, class F>
  void Bind(const T& a, const F& boundObject) {
    Get_val_call = std::bind(a, std::ref(boundObject));
  };

  Variant GetData() {
    assert(Get_val_call);
    return Get_val_call();
  }
};

class NNodePinInput : public NNodePinBase {
 protected:
 public:
  NNodePinInput() : NNodePinBase(){};
  virtual ~NNodePinInput() = default;
};

/**
 * データ出力用のピン
 */
class NNodePinOutput : public NNodePinBase {
 protected:
  // THINK:Needless?
  /*!値の出力先のピン*/

 public:
  NNodePinOutput() : NNodePinBase(){};
  virtual ~NNodePinOutput() = default;
  Variant Calculate() { return "Success"; }
};

class NNodePinExecution : public NNodePinBase {
 private:
  std::vector<NUUID> parents;
  NUUID target;

  using ParentNodeRef = ParentRef<NEventBase>;
  ParentNodeRef pnodeRef;

 public:
  explicit NNodePinExecution() : NNodePinBase(){};
  virtual ~NNodePinExecution() = default;
  void Triggered() { pnodeRef.lock()->DoAction(); };
};

class NNodeExecutionTest1 : public NNodeBase {
 protected:
 public:
  NNodeExecutionTest1() : NNodeBase() {
    auto ipin = std::make_unique<NNodePinInput>();
    auto opin = std::make_unique<NNodePinOutput>();

    ipin->Bind(&NNodePinOutput::Calculate, opin);
    pinInput.emplace_back(std::move(ipin));
    pinOutput.emplace_back(std::move(opin));
  }

  Variant func() { return pinInput.front()->GetData(); }
  virtual ~NNodeExecutionTest1() = default;
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
  virtual void Execute() override;
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
  virtual void Execute() override{};
};

class NEventImpl : public NEventBase {
 private:
 protected:
 public:
  explicit NEventImpl() = default;
  virtual ~NEventImpl() = default;
};

class NSequencer : public NGraphBase {
 private:
 protected:
 public:
  explicit NSequencer() = default;
  virtual ~NSequencer() = default;
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