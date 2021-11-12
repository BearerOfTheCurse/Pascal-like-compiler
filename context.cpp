#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <set>
#include <unordered_map>
#include <iostream>
#include "util.h"
#include "cpputil.h"
#include "node.h"
#include "type.h"
#include "symbol.h"
#include "symtab.h"
#include "ast.h"
#include "astvisitor.h"
#include "context.h"
#include "cfg.h"
#include "insgenerator.h"
#include "irprinter.h"
#include "codegenerator.h"
using namespace std;

////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////

struct Context
{
private:
  SymbolTable *myTable;
  ASTVisitor *visitor;
  InstructionSequence *insSet;
  CodeGenerator *codeGen;

  bool build;
  struct Node *root;
  Type *intType;
  Type *charType;

  char flag;

  void printSymbol(Symbol *input);
  void printSymbolTable(SymbolTable *curTable);

  void printRecord(Type *input, bool isType);
  void printLast(Type *input, bool isType);

public:
  Context(struct Node *ast);
  ~Context();

  void set_flag(char flag);

  void build_symtab();

  void build_insSet();
  void print_insSet();

  void build_code();
  void print_code();

  // TODO: additional methods
};

// TODO: helper classes (e.g., SymbolTableBuilder)

////////////////////////////////////////////////////////////////////////
// Context class implementation
////////////////////////////////////////////////////////////////////////

Context::Context(struct Node *ast)
{
  intType = new Type(INTEGER);
  charType = new Type(CHAR);

  myTable = new SymbolTable(intType, charType);
  root = ast;

  insSet = new InstructionSequence();
}

Context::~Context()
{
}

void Context::set_flag(char flag)
{
  this->flag = flag;
}

void Context::build_insSet()
{
  InsGenerator *insgen = new InsGenerator(myTable, insSet);

  insgen->visit(root);
}

void Context::print_insSet()
{
  IRPrinter *printer = new IRPrinter(insSet);
  printer->print();
}

void Context::build_code()
{
  codeGen = new CodeGenerator(myTable, insSet);
  codeGen->goThrough();
}

void Context::print_code()
{
  codeGen->printCode();
}

void Context::build_symtab()
{
  build = 1;
  visitor = new ASTVisitor(build, myTable);
  if (flag == 's')
  {
    visitor->setIsPrint(1);
  }

  visitor->visit(root);
}

////////////////////////////////////////////////////////////////////////
// Context API functions
////////////////////////////////////////////////////////////////////////

struct Context *context_create(struct Node *ast)
{
  return new Context(ast);
}

void context_destroy(struct Context *ctx)
{
  delete ctx;
}

void context_set_flag(struct Context *ctx, char flag)
{
  ctx->set_flag(flag);
}

void context_build_symtab(struct Context *ctx)
{
  ctx->build_symtab();
}

void context_build_IR(struct Context *ctx)
{
  ctx->build_insSet();
}

void context_print_IR(struct Context *ctx)
{
  ctx->print_insSet();
}

void context_build_code(struct Context *ctx)
{
  ctx->build_code();
}

void context_print_code(struct Context *ctx)
{
  ctx->print_code();
}

void context_check_types(struct Context *ctx)
{
}
