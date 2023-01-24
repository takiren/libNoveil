#pragma once

#include <memory>

#include <queue>

namespace noveil {

// ƒOƒ‰ƒt
class NGraphBase;

// ’¸“_
struct NNodeBase;

// T
struct NNodeTemplate;

struct NNodeBase {

  NNodeBase() = default;
  virtual ~NNodeBase() = default;
};

struct NNodeTemplate : NNodeBase {
  NNodeTemplate() : NNodeBase(){};
};

class NGraphBase {
 public:
  NGraphBase() = default;
  virtual ~NGraphBase() = default;
};

class NGraphTemplate : public NGraphBase {
 public:
};


class NSequencer : public NGraphBase {
 private:
 protected:
 public:
  NSequencer() = default;
  virtual ~NSequencer() = default;
};
}  // namespace noveil