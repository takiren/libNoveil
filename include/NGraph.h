#pragma once
/**
 * @file NGraph.h
 * @brief �O���t����
 * @author takiren
 *
 */

#include <functional>
#include <memory>
#include <queue>

#include "NUtils.h"
#include "NVariant.h"

namespace noveil {

// �O���t
class NGraphBase;

// ���_
class NNodeBase;

// �C�x���g
class NEventBase;
class NEventImpl;

// �s��
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

/*!�C���v�b�g�s���̃f�[�^���M��*/
using SourcePinRef = std::weak_ptr<NNodePinOutput>;

/*!�A�E�g�v�b�g�s���̃f�[�^���M��*/
using TargetPinRef = std::weak_ptr<NNodePinInput>;
using PackedTargetPinRef = std::vector<TargetPinRef>;

/**�O���t�̒��_�ƂȂ�*/
class NNodeBase : public INUid, private Noncopyable {
 private:
 protected:
  std::vector<ChildRef<NNodePinInput>> pinInput;
  std::vector<ChildRef<NNodePinOutput>> pinOutput;

 public:
  explicit NNodeBase() : INUid(){};
  virtual ~NNodeBase() = default;
};

/**��������Ȃ�*/
class NNodeTemplate : public NNodeBase {
 public:
  explicit NNodeTemplate() : NNodeBase(){};
  virtual ~NNodeTemplate() = default;
};

/**
 *�m�[�h���Ǘ����邽�߂̊��N���X�B
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
 * �m�[�h�̒l���o�͂ɕK�v�ȃI�u�W�F�N�g:=�s���̊��N���X�B
 */
class NNodePinBase : public INUid, private Noncopyable {
 private:
  /** std::variant�̃��b�p�[�N���X*/
  Variant mValue;

  /**
   * �s����ێ�����NNodeBase�̎Q��
   */
  using ParentNodeRef = ParentRef<NNodeBase>;

  /*!�s����ێ�����NNodeBase�̎Q��*/
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
 * �f�[�^�o�͗p�̃s��
 */
class NNodePinOutput : public NNodePinBase {
 protected:
  /*!�l�̏o�͐�̃s��*/
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