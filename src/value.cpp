#include "../include/value.h"
#include <sstream>
#include <stdexcept>


Value *IntValue::apply_operator(Token t, Value *to) {
  //  Unary Operator
  if (!to) {
    switch (t.type) {
    case NOT:
      throw std::runtime_error("Invalid operands for expression: !'int'");
    case NEGATIVE:
      return new IntValue(-this->value);
    case INCREMENT:
      return new IntValue(this->value + 1);
    case DECREMENT:
      return new IntValue(this->value - 1);
    case RETURN:
      return this;
    default: throw std::runtime_error("Invalid operator");
    }
  }
  auto *is_string = dynamic_cast<StringValue *>(to);
  auto *is_bool = dynamic_cast<BoolValue *>(to);
  auto *is_float = dynamic_cast<FloatValue *>(to);

  if (t.type == ASSIGN) {
    if (is_string) {
      return new StringValue(is_string->value);
    } else if (is_bool) {
      return new BoolValue(is_bool->value);
    } else if (is_float) {
      return new FloatValue(is_float->value);
    } else {
      return new IntValue(dynamic_cast<IntValue *>(to)->value);
    }
  }

  std::ostringstream msg;
  msg << "Invalid operands for expression: 'int' " << t.value << " '" << to->get_type() << "'";

  // Cannot apply an operation from an integer to a string or boolean
  if (is_string || is_bool) {
    throw std::runtime_error(msg.str());
  }

  switch (t.type) {
  case ADD:
  case ASSIGN_ADD:
    if (is_float) {
      return new FloatValue(this->value + is_float->value);
    } else {
      return new IntValue(this->value + dynamic_cast<IntValue *>(to)->value);
    }
  case SUBTRACT:
  case ASSIGN_SUBTRACT:
    if (is_float) {
      return new FloatValue(this->value - is_float->value);
    } else {
      return new IntValue(this->value - dynamic_cast<IntValue *>(to)->value);
    }
  case MULTIPLY:
  case ASSIGN_MULTIPLY:
    if (is_float) {
      return new FloatValue(this->value * is_float->value);
    } else {
      return new IntValue(this->value * dynamic_cast<IntValue *>(to)->value);
    }
  case DIVIDE:
  case ASSIGN_DIVIDE:
    if (is_float) {
      return new FloatValue(this->value / is_float->value);
    } else {
      return new IntValue(this->value / dynamic_cast<IntValue *>(to)->value);
    }
  case MODULO:
  case ASSIGN_MODULO:
    if (is_float) {
      throw std::runtime_error(msg.str());
    } else {
      return new IntValue(this->value % dynamic_cast<IntValue *>(to)->value);
    }
  case AND:
  case OR:
    throw std::runtime_error(msg.str());
  case LESS_THAN:
    return new BoolValue(
        this->value <
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  case GREATER_THAN:
    return new BoolValue(
        this->value >
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  case LESS_THAN_OR_EQUAL:
    return new BoolValue(
        this->value <=
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  case GREATER_THAN_OR_EQUAL:
    return new BoolValue(
        this->value >=
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  case EQUAL:
    return new BoolValue(
        this->value ==
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  case NOT_EQUAL:
    return new BoolValue(
        this->value !=
        (is_float ? is_float->value : dynamic_cast<IntValue *>(to)->value));
  default:
    throw std::runtime_error(msg.str());
  }
}

Value *FloatValue::apply_operator(Token t, Value *to) {
  if (!to) {
    switch (t.type) {
    case NOT:
      throw std::runtime_error("Invalid operands for expression: !'int'");
    case NEGATIVE:
      return new IntValue(-this->value);
    case INCREMENT:
      return new IntValue(this->value + 1);
    case DECREMENT:
      return new IntValue(this->value - 1);
    case RETURN:
      return this;
    default: throw std::runtime_error("Invalid operator");
    }
  }

  auto *is_string = dynamic_cast<StringValue *>(to);
  auto *is_bool = dynamic_cast<BoolValue *>(to);
  auto *is_int = dynamic_cast<IntValue *>(to);

  if (t.type == ASSIGN) {
    if (is_string) {
      return new StringValue(is_string->value);
    } else if (is_bool) {
      return new BoolValue(is_bool->value);
    } else if (is_int) {
      return new IntValue(is_int->value);
    } else {
      return new FloatValue(dynamic_cast<FloatValue *>(to)->value);
    }
  }

  std::ostringstream msg;
  msg << "Invalid operands for expression: 'float' " << t.value << " '" << to->get_type() << "'";

  // Cannot apply an operation from a float to a string or boolean
  if (is_string || is_bool) {
    throw std::runtime_error(msg.str());
  }

  switch (t.type) {
  case ADD:
  case ASSIGN_ADD:
    if (is_int) {
      return new FloatValue(this->value + is_int->value);
    } else {
      return new FloatValue(this->value + dynamic_cast<FloatValue *>(to)->value);
    }
  case SUBTRACT:
  case ASSIGN_SUBTRACT:
    if (is_int) {
      return new FloatValue(this->value - is_int->value);
    } else {
      return new IntValue(this->value - dynamic_cast<FloatValue *>(to)->value);
    }
  case MULTIPLY:
  case ASSIGN_MULTIPLY:
    if (is_int) {
      return new FloatValue(this->value * is_int->value);
    } else {
      return new IntValue(this->value * dynamic_cast<FloatValue *>(to)->value);
    }
  case DIVIDE:
  case ASSIGN_DIVIDE:
    if (is_int) {
      return new FloatValue(this->value / is_int->value);
    } else {
      return new IntValue(this->value / dynamic_cast<FloatValue *>(to)->value);
    }
  case MODULO:
  case ASSIGN_MODULO:
    throw std::runtime_error(msg.str());
  case NOT:
    throw std::runtime_error("Invalid operands for expression: !'float'");
  case NEGATIVE:
    return new FloatValue(-this->value);
  case INCREMENT:
    return new FloatValue(this->value + 1.0);
  case DECREMENT:
    return new FloatValue(this->value - 1.0);
  case RETURN:
    return this;
  case AND:
  case OR:
    throw std::runtime_error(msg.str());
  case LESS_THAN:
    return new BoolValue(
        this->value <
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  case GREATER_THAN:
    return new BoolValue(
        this->value >
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  case LESS_THAN_OR_EQUAL:
    return new BoolValue(
        this->value <=
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  case GREATER_THAN_OR_EQUAL:
    return new BoolValue(
        this->value >=
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  case EQUAL:
    return new BoolValue(
        this->value ==
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  case NOT_EQUAL:
    return new BoolValue(
        this->value !=
        (is_int ? is_int->value : dynamic_cast<FloatValue *>(to)->value));
  default:
    throw std::runtime_error(msg.str());
  }
}

Value *StringValue::apply_operator(Token t, Value *to) {
  if (t.type != ADD && t.type != ASSIGN_ADD && t.type != ASSIGN &&
      t.type != EQUAL && t.type != NOT_EQUAL) {
    std::ostringstream msg;
    msg << "Invalid operands for expression: 'string' " << t.value << " '" << to->get_type() << "'";
    throw std::runtime_error(msg.str());
  }

  auto *is_string = dynamic_cast<StringValue *>(to);

  if (t.type == ADD || t.type == ASSIGN_ADD) {
    std::string temp(this->value);
    temp += to->to_string();
    return new StringValue(temp);
  } else if (t.type == ASSIGN) {
    if (auto *is_int = dynamic_cast<IntValue *>(to)) {
      return new IntValue(is_int->value);
    } else if (auto *is_bool = dynamic_cast<BoolValue *>(to)) {
      return new BoolValue(is_bool->value);
    } else if (auto *is_float = dynamic_cast<FloatValue *>(to)) {
      return new FloatValue(is_float->value);
    } else {
      return new StringValue(dynamic_cast<StringValue *>(to)->value);
    }

  } else if (t.type == EQUAL) {
    if (is_string) {
      return new BoolValue(this->value == is_string->value);
    } else {
      return new BoolValue(false);
    }

  } else {
    if (is_string) {
      return new BoolValue(this->value != is_string->value);
    } else {
      return new BoolValue(true);
    }
  }
}


Value *BoolValue::apply_operator(Token t, Value *to) {
  auto *is_string = dynamic_cast<StringValue *>(to);
  auto *is_float = dynamic_cast<FloatValue *>(to);
  auto *is_int = dynamic_cast<IntValue *>(to);

  if (t.type == ASSIGN) {
    if (is_string) {
      return new StringValue(is_string->value);
    } else if (is_float) {
      return new FloatValue(is_float->value);
    } else if (is_int) {
      return new IntValue(is_int->value);
    } else {
      return new BoolValue(dynamic_cast<BoolValue *>(to)->value);
    }
  }

  std::ostringstream msg;
  msg << "Invalid operands for expression: 'bool' " <<  t.value << " '" << to->get_type() << "'";

  if (is_int || is_float || is_string) {
    throw std::runtime_error(msg.str());
  }

  switch (t.type) {
  case NOT:
    return new BoolValue(!this->value);
  case RETURN:
    return this;
  case AND:
    return new BoolValue(this->value && dynamic_cast<BoolValue *>(to)->value);
  case OR:
    return new BoolValue(this->value || dynamic_cast<BoolValue *>(to)->value);
  case LESS_THAN:
    return new BoolValue(this->value < dynamic_cast<BoolValue *>(to)->value);
  case GREATER_THAN:
    return new BoolValue(this->value > dynamic_cast<BoolValue *>(to)->value);
  case LESS_THAN_OR_EQUAL:
    return new BoolValue(this->value <= dynamic_cast<BoolValue *>(to)->value);
  case GREATER_THAN_OR_EQUAL:
    return new BoolValue(this->value >= dynamic_cast<BoolValue *>(to)->value);
  case EQUAL:
    return new BoolValue(this->value == dynamic_cast<BoolValue *>(to)->value);
  case NOT_EQUAL:
    return new BoolValue(this->value != dynamic_cast<BoolValue *>(to)->value);
  default:
    throw std::runtime_error(msg.str());
  }
}

Value *VoidValue::apply_operator(Token t, Value *to) {
  throw std::runtime_error("Cannot evaluate type 'void'");
}


std::string IntValue::to_string() const {
  return std::to_string(value);
}

std::string IntValue::get_type() const {
  return "int";
}


std::string FloatValue::to_string() const {
  return std::to_string(value);
}

std::string FloatValue::get_type() const {
  return "float";
}

std::string StringValue::to_string() const {
  return value;
}

std::string StringValue::get_type() const {
  return "string";
}

std::string BoolValue::to_string() const {
  return std::to_string(value);
}

std::string BoolValue::get_type() const {
  return "bool";
}

std::string VoidValue::to_string() const {
  return "";
}

std::string VoidValue::get_type() const {
  return "void";
}
