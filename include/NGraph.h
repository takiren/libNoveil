#pragma once
/**
 * @file NGraph.h
 * @brief グラフ実装
 * @author takiren
 *
 */

#include <functional>
#include <memory>
#include <queue>

#include "NUtils.h"
#include "NVariant.h"

namespace noveil {

// グラフ
class NGraphBase;

// 頂点
class NNodeBase;

// イベント
class NEventBase;
class NEventImpl;

// ピン
class NNodePinBase;
class NNodePinInput;
class NNodePinOutput;
class NNodePinExecution;
// T
class NNodeTemplate;

/*
 * 弱参照
 */
template <typename T>
using ParentRef = std::weak_ptr<T>;

/*
 * 強参照
 */
template <typename T>
using ChildRef = std::shared_ptr<T>;

/*!インプットピンのデータ送信元*/
using SourcePinRef = std::weak_ptr<NNodePinOutput>;

/*!アウトプットピンのデータ送信先*/
using TargetPinRef = std::weak_ptr<NNodePinInput>;
using PackedTargetPinRef = std::vector<TargetPinRef>;

using CallbackFunction = std::function<Variant(void)>;

/**グラフの頂点となる*/
class NNodeBase : public INInfo, private Noncopyable {
 private:
  bool bIsExecutionNode;

 protected:
  std::vector<ChildRef<NNodePinInput>> pinInput;
  std::vector<ChildRef<NNodePinOutput>> pinOutput;

 public:
  explicit NNodeBase() : INInfo(), bIsExecutionNode(false){};
  virtual ~NNodeBase() = default;

  virtual void AddInputNode(){};
  virtual void AddOutputNode(){};

  virtual void Execute(){};
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
  explicit NGraphBase() : INInfo(){};
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

  /*!Binds a functor which type is std::function<Variant(void)>.*/
  void Bind(const CallbackFunction&& rhs) { Get_val_call = rhs; };

  /*!Binds like std::bind(&Hoge::fuga,std::ref(pointer))*/
  template <class T, class F>
  void Bind(T&& a, F&& boundObject) {
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
  TargetPinRef targetPin;

 public:
  NNodePinOutput() : NNodePinBase(){};
  NNodePinOutput(TargetPinRef target) : NNodePinBase(), targetPin(target){};
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


class NNodeExecutor : public NNodeBase {
 protected:
 public:
  NNodeExecutor() = default;
  virtual ~NNodeExecutor() = default;

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

}  // namespace noveil