#pragma once
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

using SourcePinRef = std::weak_ptr<NNodePinOutput>;
using TargetPinRef = std::weak_ptr<NNodePinInput>;

// ピンのコンテナ
class NNodeBase : public INUid, private Noncopyable {
 private:
 protected:
  std::vector<ChildRef<NNodePinInput>> pinInput;
  std::vector<ChildRef<NNodePinOutput>> pinOutput;

 public:
  explicit NNodeBase() : INUid(){};
  virtual ~NNodeBase() = default;
};

class NNodeTemplate : public NNodeBase {
 public:
  explicit NNodeTemplate() : NNodeBase(){};
  virtual ~NNodeTemplate() = default;
};

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

class NNodePinBase : public INUid, private Noncopyable {
 private:
  Variant mValue;
  using ParentNodeRef = ParentRef<NNodeBase>;
  ParentNodeRef parentNode;

 protected:
  std::vector<std::string> allowedList;

 public:
  explicit NNodePinBase() : INUid(){};
  virtual ~NNodePinBase() = default;
};

class NNodePinInput : public NNodePinBase {
 protected:
  SourcePinRef sourcePin;

 public:
  NNodePinInput() : NNodePinBase(){};
  NNodePinInput(SourcePinRef source) : NNodePinBase(), sourcePin(source){};
  virtual ~NNodePinInput() = default;

  Variant& GetData() { return Variant(); }
};

class NNodePinOutput : public NNodePinBase {
 protected:
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

  Variant func() { return pinOutput.front().lock()->Calculate(); }
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