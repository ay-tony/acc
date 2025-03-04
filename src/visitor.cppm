export module visitor;

import std;
import antlr4;
import symbol;
import error_handler;

export class visitor : public sysy_base_visitor {
private:
  std::size_t m_indent{};
  std::string m_ir{};
  variable::ir_cnt_t m_ir_cnt{};
  std::size_t m_label_cnt{};
  std::optional<std::string> m_current_function_name;
  std::vector<scope> m_scopes{1};

private:
  struct expression {
    bool is_const;
    variable::TYPE type;
    variable::ir_cnt_t ir_cnt;               // valid only non-const
    std::variant<std::int32_t, float> value; // valid only const

    std::string to_string();
    expression to_i1(this expression &&self, visitor &vis);
  };

private:
  void pd();
  template <class... Args> void p(std::format_string<Args...> fmt, Args &&...args);
  template <class... Args> void pl(std::format_string<Args...> fmt, Args &&...args);

  variable::ir_cnt_t new_ir_cnt() { return ++m_ir_cnt; };
  std::string new_label() { return std::format("label_{}", ++m_label_cnt); }

  scope &current_scope() { return m_scopes.back(); }
  variable resolve_variable(const std::string &name);
  function resolve_function(const std::string &name);

  expression expression_cast(expression raw_expression, variable::TYPE target_type, bool to_non_const = false);
  variable::TYPE get_common_type(variable::TYPE type1, variable::TYPE type2, std::string op);

public:
  visitor() = default;

  std::string_view ir() { return m_ir; };

  virtual std::any visitFunctionDefinition(sysy_parser::FunctionDefinitionContext *ctx) override;
  virtual std::any visitTerminal(antlr4::tree::terminal_node *ctx) override;
  virtual std::any visitLeftValue(sysy_parser::LeftValueContext *ctx) override;
  virtual std::any visitBlock(sysy_parser::BlockContext *ctx) override;

  virtual std::any visitLeftValueExpression(sysy_parser::LeftValueExpressionContext *ctx) override;
  virtual std::any visitBraceExpression(sysy_parser::BraceExpressionContext *ctx) override;
  virtual std::any visitUnaryExpression(sysy_parser::UnaryExpressionContext *ctx) override;
  virtual std::any visitIntegerConstantExpression(sysy_parser::IntegerConstantExpressionContext *ctx) override;
  virtual std::any visitFloatingConstantExpression(sysy_parser::FloatingConstantExpressionContext *ctx) override;
  virtual std::any visitBinaryArithmeticExpression(sysy_parser::BinaryArithmeticExpressionContext *ctx) override;
  virtual std::any visitBinaryLogicExpression(sysy_parser::BinaryLogicExpressionContext *ctx) override;
  virtual std::any visitBinaryRelationExpression(sysy_parser::BinaryRelationExpressionContext *ctx) override;

  virtual std::any visitExpressionStatement(sysy_parser::ExpressionStatementContext *ctx) override;
  virtual std::any visitBlockStatement(sysy_parser::BlockStatementContext *ctx) override;
  virtual std::any visitAssignmentStatement(sysy_parser::AssignmentStatementContext *ctx) override;
  virtual std::any visitReturnStatement(sysy_parser::ReturnStatementContext *ctx) override;
  virtual std::any visitIfStatement(sysy_parser::IfStatementContext *ctx) override;

  virtual std::any visitConstDeclaration(sysy_parser::ConstDeclarationContext *ctx) override;
  virtual std::any visitConstDefinition(sysy_parser::ConstDefinitionContext *ctx) override;
  virtual std::any visitConstInitializeValue(sysy_parser::ConstInitializeValueContext *ctx) override;
  virtual std::any visitVariableDeclaration(sysy_parser::VariableDeclarationContext *ctx) override;
  virtual std::any visitVariableDefinition(sysy_parser::VariableDefinitionContext *ctx) override;
  virtual std::any visitInitializeValue(sysy_parser::InitializeValueContext *ctx) override;
};
