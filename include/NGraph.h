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

template <typename T>
using ParentRef = std::weak_ptr<T>;

template <typename T>
using ChildRef = std::weak_ptr<T>;

/*!インプットピンのデータ送信元*/
using SourcePinRef = std::weak_ptr<NNodePinOutput>;

/*!アウトプットピンのデータ送信先*/
using TargetPinRef = std::weak_ptr<NNodePinInput>;
using PackedTargetPinRef = std::vector<TargetPinRef>;

/**グラフの頂点となる*/
class NNodeBase : public INUid, private Noncopyable {
 private:
 protected:
  std::vector<ChildRef<NNodePinInput>> pinInput;
  std::vector<ChildRef<NNodePinOutput>> pinOutput;

 public:
  explicit NNodeBase() : INUid(){};
  virtual ~NNodeBase() = default;
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
class NGraphBase : public INUid, private Noncopyable {
 private:
 public:
  explicit NGraphBase() : INUid(){};
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
class NNodePinBase : public INUid, private Noncopyable {
 private:
  /** std::variantのラッパークラス*/
  Variant mValue;

  /**
   * ピンを保持するNNodeBaseの参照
   */
  using ParentNodeRef = ParentRef<NNodeBase>;

  /*!ピンを保持するNNodeBaseの参照*/
  ParentNodeRef parentNode;

 protected:
  std::function<Variant()> sourceFunc;
  std::vector<std::string> allowedList;

 public:
  explicit NNodePinBase() : INUid(){};
  virtual ~NNodePinBase() = default;

  void SetFunctor(std::function<Variant(void)>& rhs){
      sourceFunc
  };

  virtual Variant& Get(){};
};

class NNodePinInput : public NNodePinBase {
 protected:
  using PackedSourcePinRef = std::vector<SourcePinRef>;
  SourcePinRef sourcePin;

 public:
  NNodePinInput() : NNodePinBase(){};
  NNodePinInput(SourcePinRef source) : NNodePinBase(), sourcePin(source){};
  virtual ~NNodePinInput() = default;
  Variant& GetData() {
    assert(sourcePin.expired());
    return sourcePin.lock().get()->Calculate();
  }
};
/**
 * データ出力用のピン
 */
class NNodePinOutput : public NNodePinBase {
 protected:
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
    auto ipin = std::make_shared<NNodePinInput>();
    auto* te = new NNodePinOutput();
    auto opin = std::make_shared<NNodePinOutput>(ipin);
    pinInput.emplace_back(ipin);
    pinOutput.emplace_back(opin);
  }

  Variant func() { return pinInput.front().lock()->GetData(); }
  virtual ~NNodeExecutionTest1() = default;
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