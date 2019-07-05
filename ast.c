/**
 * @file ast.c is a Abstract Syntax Tree
 * @author Selman ALPÜNDAR(selman.alp@hotmail.com.tr)
 * @brief 
 * @version 0.1
 * @date 2019-06-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "y.tab.h"
#include "utils.h"


/**
 * @brief It gives the type name of the value type.
 * 
 * @param t is a value type.
 * @return const char*  is a type name.
 */
const char *type_name(enum value_type t) {
  switch (t) {
    case INTEGER: return "int";
    case BOOLEAN: return "bool";
    case ERROR: return "error";
    default: return "not-a-type";
  }
}

/*!
 * @brief allocate a bool litee
 * 
 * @param v 
 * @return struct expr* 
 */
struct expr* bool_lit(int v) {
  struct expr* r = malloc(sizeof(struct expr));
  r->type = BOOL_LIT;
  r->value = v;
  return r;
}

/**
 * @brief 
 * It takes a  value to create a new integer literal.
 * @param v is a value of integer literal.
 * @return struct expr* is an expression.
 */
struct expr* literal(int v) {
  struct expr* r = malloc(sizeof(struct expr));
  r->type = LITERAL;
  r->value = v;
  return r;
}


/**
 * @brief 
 * It takes an identifier to create new variable with it.
 * @param id is an identifier for variable.
 * @return struct expr* is an expression.
 */
struct expr* variable(size_t id) {
  struct expr* r = malloc(sizeof(struct expr));
  r->type = VARIABLE;
  r->id = id;
  return r;
}

/**
 * @brief 
 * It takes left-hand side, right hand side expression and operatin identifier to create new expression.
 * @param lhs is a left-hand side expression.
 * @param op  is a operation identifier.
 * @param rhs is a right-hand side expression.
 * @return struct expr*  is an expression.
 */
struct expr* binop(struct expr *lhs, int op, struct expr *rhs) {
  struct expr* r = malloc(sizeof(struct expr));
  r->type = BIN_OP;
  r->binop.lhs = lhs;
  r->binop.op = op;
  r->binop.rhs = rhs;
  return r;
}

/**
 * @brief 
 * IT takes an expression to print.
 * @param expr is an expression
 */
void print_expr(struct expr *expr) {
  switch (expr->type) {
    case BOOL_LIT:
      printf("%s", expr->value ? "true" : "false");
      break;

    case LITERAL:
      printf("%d", expr->value);
      break;

    case VARIABLE:
      printf("%s", string_int_rev(&global_ids, expr->id));
      break;

    case BIN_OP:
      printf("(");
      print_expr(expr->binop.lhs);
      switch (expr->binop.op) {
        case EQ: printf(" == "); break;
        case NE: printf(" != "); break;
        case GE: printf(" >= "); break;
        case LE: printf(" <= "); break;
        case AND: printf(" && "); break;
        default: printf(" %c ", expr->binop.op); break;
      }
      print_expr(expr->binop.rhs);
      printf(")");
      break;  
  }
}

/**
 * @brief 
 * it print a number as indent to print space.
 * @param indent is a number of space.
 */
static void print_indent(int indent) {
  while (indent--) {
    printf("  ");
  }
}

/**
 * @brief takes an statement and print it
 * 
 * @param stmt 
 * @param indent  
 */
void print_stmt(struct stmt *stmt, int indent) {
  switch (stmt->type) {
    case STMT_SEQ:
      print_stmt(stmt->seq.fst, indent);
      print_stmt(stmt->seq.snd, indent);
      break;

    case STMT_ASSIGN:
      print_indent(indent);
      printf("%s = ", string_int_rev(&global_ids, stmt->assign.id));
      print_expr(stmt->assign.expr);
      printf(";\n");
      break;

    case STMT_PRINT:
      print_indent(indent);
      printf("print ");
      print_expr(stmt->print.expr);
      printf(";\n");
      break;

    case STMT_WHILE:
      print_indent(indent);
      printf("while (");
      print_expr(stmt->while_.cond);
      printf(") {\n");
      print_stmt(stmt->while_.body, indent + 1);
      print_indent(indent);
      printf("}\n");
      break;

    case STMT_IF:
      print_indent(indent);
      printf("if (");
      print_expr(stmt->ifelse.cond);
      printf(") {\n");
      print_stmt(stmt->ifelse.if_body, indent + 1);
      if (stmt->ifelse.else_body) {
        print_indent(indent);
        printf("} else {\n");
        print_stmt(stmt->ifelse.else_body, indent + 1);
      }
      print_indent(indent);
      printf("}\n");
      break;

    case STMT_INCREMENT:
      print_indent(indent);
      printf("++");
      print_expr(stmt->increment.expr);
      break;

    case STMT_DECREMENT:
      print_indent(indent);
      printf("--");
      print_expr(stmt->decrement.expr);
      break;

    default:
      printf("Default");
      
  }
}


/**
 * @brief 
 * 
 * @param expr 
 */
void emit_stack_machine(struct expr *expr) {
  switch (expr->type) {
    case BOOL_LIT:
      printf(expr->value ? "load_true\n" : "load_false\n");
      break;

    case LITERAL:
      printf("load_imm %d\n", expr->value);
      break;

    case VARIABLE:
      printf("load_mem %zu # %s\n", expr->id, string_int_rev(&global_ids, expr->id));
      break;

    case BIN_OP:
      emit_stack_machine(expr->binop.lhs);
      emit_stack_machine(expr->binop.rhs);
      switch (expr->binop.op) {
        case '+': printf("add\n"); break;
        case '-': printf("sub\n"); break;
        case '*': printf("mul\n"); break;
        case '/': printf("div\n"); break;

        case EQ: printf("eq\n"); break;
        case NE: printf("ne\n"); break;

        case GE: printf("ge\n"); break;
        case LE: printf("le\n"); break;
        case '>': printf("gt\n"); break;
        case '<': printf("lt\n"); break;
        case  AND: printf("and\n"); break;
      }
      break;
  }
}


static int next_reg = 0;

/**
 * @brief 
 * It cretes new register.
 * @return int is a register. 
 */
static int gen_reg() {
  return next_reg++;
}

/**
 * @brief 
 * It takes an expression to emit register machine.
 * @param expr is an expression.
 * @return int is a register.
 */
int emit_reg_machine(struct expr *expr) {
  int result_reg = gen_reg();
  switch (expr->type) {
    case BOOL_LIT:
      printf("r%d = %d\n", result_reg, expr->value);
      break;

    case LITERAL:
      printf("r%d = %d\n", result_reg, expr->value);
      break;

    case VARIABLE:
      printf("r%d = load %zu # %s\n", result_reg, expr->id, string_int_rev(&global_ids, expr->id));
      break;

    case BIN_OP: {
      int lhs = emit_reg_machine(expr->binop.lhs);
      int rhs = emit_reg_machine(expr->binop.rhs);
      switch (expr->binop.op) {
        case '+': printf("r%d = add r%d, r%d\n", result_reg, lhs, rhs); break;
        case '-': printf("r%d = sub r%d, r%d\n", result_reg, lhs, rhs); break;
        case '*': printf("r%d = mul r%d, r%d\n", result_reg, lhs, rhs); break;
        case '/': printf("r%d = div r%d, r%d\n", result_reg, lhs, rhs); break;

        case EQ: printf("r%d = eq r%d, r%d\n", result_reg, lhs, rhs); break;
        case NE: printf("r%d = ne r%d, r%d\n", result_reg, lhs, rhs); break;

        case GE: printf("r%d = ge r%d, r%d\n", result_reg, lhs, rhs); break;
        case LE: printf("r%d = le r%d, r%d\n", result_reg, lhs, rhs); break;
        case '>': printf("r%d = gt r%d, r%d\n", result_reg, lhs, rhs); break;
        case '<': printf("r%d = lt r%d, r%d\n", result_reg, lhs, rhs); break;
      }
      break;
    }
  }
  return result_reg;
}

/**
 * @brief 
 * It takes an expression and return the value type of the expression.
 * @param expr is an expression
 * @return enum value_type  
 */
enum value_type check_types(struct expr *expr) {
  switch (expr->type) {
    case BOOL_LIT:
      return BOOLEAN;

    case LITERAL:
      return INTEGER;

    case VARIABLE: {
      LLVMValueRef ptr = vector_get(&global_types, expr->id);
      LLVMTypeRef t = LLVMGetElementType(LLVMTypeOf(ptr));
      return LLVMGetIntTypeWidth(t) == 1 ? BOOLEAN : INTEGER;
    }

    case BIN_OP: {
      enum value_type lhs = check_types(expr->binop.lhs);
      enum value_type rhs = check_types(expr->binop.rhs);
      switch (expr->binop.op) {
        case '+':
        case '-':
        case '*':
        case '/':
          if (lhs == INTEGER && rhs == INTEGER)
            return INTEGER;
          else
            return ERROR;

        case EQ:
        case NE:
          if (lhs == rhs && lhs != ERROR)
            return BOOLEAN;
          else
            return ERROR;

        case GE:
        case LE:
        case '>':
        case '<':
          if (lhs == INTEGER && rhs == INTEGER)
            return BOOLEAN;
          else
            return ERROR;
        case AND:
          if (lhs == BOOLEAN && rhs == BOOLEAN)
            return BOOLEAN;
          else if (lhs == INTEGER && rhs == INTEGER)
            return INTEGER;
          else
            return ERROR;
        case OR:
          if (lhs == BOOLEAN || rhs == BOOLEAN)
            return BOOLEAN;
          else if (lhs == INTEGER || rhs == INTEGER)
            return INTEGER;
          else
            return ERROR;
      }

      default:
        return ERROR;
    }
  }
}

/**
 * @brief 
 * It takes an expression to free it from memory.
 * @param expr  is an expression.
 */
void free_expr(struct expr *expr) {
  switch (expr->type) {
    case BOOL_LIT:
    case LITERAL:
    case VARIABLE:
      free(expr);
      break;

    case BIN_OP:
      free_expr(expr->binop.lhs);
      free_expr(expr->binop.rhs);
      free(expr);
      break;
  }
}

/**
 * @brief 
 * TODO:
 * @param fst 
 * @param snd 
 * @return struct stmt* 
 */
struct stmt* make_seq(struct stmt *fst, struct stmt *snd) {
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_SEQ;
  r->seq.fst = fst;
  r->seq.snd = snd;
  return r;
}


/**
 * @brief 
 * It takes a id and an expression to assign a expression to given id.
 * @param id is an integer value that descripte to ?..
 * @param e is an expression.
 * @return struct stmt* 
 */
struct stmt* make_assign(size_t id, struct expr *e) {
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_ASSIGN;
  r->assign.id = id;
  r->assign.expr = e;
  return r;
}

/**
 * @brief 
 * It takes an expression and a statement to create while loop.
 * @param e is an expression.
 * @param body is a statement.
 * @return struct stmt* 
 */
struct stmt* make_while(struct expr *e, struct stmt *body) {
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_WHILE;
  r->while_.cond = e;
  r->while_.body = body;
  return r;
}

/**
 * @brief 
 * It takes an expression and two statement to create if else condition. Also, to create if we assign else_body null.
 * @param e is an expression.
 * @param if_body is a statement.
 * @param else_body is a statement.
 * @return struct stmt* 
 */
struct stmt* make_ifelse(struct expr *e, struct stmt *if_body, struct stmt *else_body) {
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_IF;
  r->ifelse.cond = e;
  r->ifelse.if_body = if_body;
  r->ifelse.else_body = else_body;
  return r;
}

/**
 * @brief 
 *  It takes an expression as a condition part of the if. It also takes a statement to construct body of if condition.
 * @param e is an expresion
 * @param body is an statement to be used body of the if.
 * @return struct stmt* is a statement
 */
struct stmt* make_if(struct expr *e, struct stmt *body) {
  return make_ifelse(e, body, NULL);
}



/**
 * @brief 
 * It takes an expression to create a statement.
 * @param e is an expression.
 * @return struct stmt* is a statement.
 */
struct stmt* make_print(struct expr *e) {
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_PRINT;
  r->print.expr = e;
  return r;
}

/**
 * @brief 
 * It takes an expression to create a statement.
 * @param e is an expression.
 * @return struct stmt* is a statement.
 */
struct stmt* make_increment(struct expr *e){
  struct stmt* r = malloc(sizeof(struct stmt));
  r->type = STMT_INCREMENT;
  r->increment.expr = e;
}

/**
 * @brief 
 * It takes a expression to create a statement.
 * @param e is an expression.
 * @return struct stmt* is a statement.
 */
struct stmt* make_decrement(struct expr *e){
    struct stmt* r = malloc(sizeof(struct stmt));
    r->type = STMT_DECREMENT;
    r->decrement.expr = e;
}

/**
 * @brief 
 * It takes a statement to clear it from memeory
 * @param stmt is a statement.
 */
void free_stmt(struct stmt *stmt) {
  switch (stmt->type) {
    case STMT_SEQ:
      free_stmt(stmt->seq.fst);
      free_stmt(stmt->seq.snd);
      break;

    case STMT_ASSIGN:
      free_expr(stmt->assign.expr);
      break;

    case STMT_PRINT:
      free_expr(stmt->print.expr);
      break;

    case STMT_WHILE:
      free_expr(stmt->while_.cond);
      free_stmt(stmt->while_.body);
      break;

    case STMT_IF:
      free_expr(stmt->ifelse.cond);
      free_stmt(stmt->ifelse.if_body);
      if (stmt->ifelse.else_body)
        free_stmt(stmt->ifelse.else_body);
      break;
    case STMT_INCREMENT:
      free_expr(stmt->increment.expr);
      break;
    case STMT_DECREMENT:
      free_expr(stmt->decrement.expr);
      break;
  }

  free(stmt);
}

/**
 * @brief 
 * It takes a statement and check If it is valid statement or not.
 * @param stmt is a statement.
 * @return int is used as a bool value zero or one.
 */
int valid_stmt(struct stmt *stmt) {
  switch (stmt->type) {
    case STMT_SEQ:
      return valid_stmt(stmt->seq.fst) && valid_stmt(stmt->seq.snd);

    case STMT_ASSIGN:
      // should the language/compiler forbid accessing uninitialized variables?
      // maybe also warn about dead assignments?
      return check_types(stmt->assign.expr) != ERROR;

    case STMT_PRINT:
      return check_types(stmt->print.expr) != ERROR;

    case STMT_WHILE:
      return check_types(stmt->while_.cond) == BOOLEAN && valid_stmt(stmt->while_.body);

    case STMT_IF:
      return
        check_types(stmt->ifelse.cond) == BOOLEAN &&
        valid_stmt(stmt->ifelse.if_body) &&
        (stmt->ifelse.else_body == NULL || valid_stmt(stmt->ifelse.else_body));

    case STMT_INCREMENT:
         return check_types(stmt->increment.expr) != ERROR;
         
    case STMT_DECREMENT:
         return check_types(stmt->decrement.expr) != ERROR;
 
  }
}

/**
 * @brief 
 * It takes a expression and generete code for it.
 * @param expr is an expression.
 * @param module is a LLVMModuleRef.
 * @param builder is a LLVMBuilderRef.
 * @return LLVMValueRef
 */
LLVMValueRef codegen_expr(struct expr *expr, LLVMModuleRef module, LLVMBuilderRef builder) {
  switch (expr->type) {
    case BOOL_LIT:
      return LLVMConstInt(LLVMInt1Type(), expr->value, 0);

    case LITERAL:
      return LLVMConstInt(LLVMInt32Type(), expr->value, 0);

    case VARIABLE:
      return LLVMBuildLoad(builder, vector_get(&global_types, expr->id), "loadtmp");

    case BIN_OP: {
      LLVMValueRef lhs = codegen_expr(expr->binop.lhs, module, builder);
      LLVMValueRef rhs = codegen_expr(expr->binop.rhs, module, builder);
      switch (expr->binop.op) {
        case '+': return LLVMBuildAdd(builder, lhs, rhs, "addtmp");
        case '-': return LLVMBuildSub(builder, lhs, rhs, "subtmp");
        case '*': return LLVMBuildMul(builder, lhs, rhs, "multmp");
        case '/': return LLVMBuildSDiv(builder, lhs, rhs, "divtmp");

        case EQ: return LLVMBuildICmp(builder, LLVMIntEQ, lhs, rhs, "eqtmp");
        case NE: return LLVMBuildICmp(builder, LLVMIntNE, lhs, rhs, "netmp");

        case GE: return LLVMBuildICmp(builder, LLVMIntSGE, lhs, rhs, "getmp");
        case LE: return LLVMBuildICmp(builder, LLVMIntSLE, lhs, rhs, "letmp");
        case '>': return LLVMBuildICmp(builder, LLVMIntSGT, lhs, rhs, "gttmp");
        case '<': return LLVMBuildICmp(builder, LLVMIntSLT, lhs, rhs, "lttmp");
        case AND: return LLVMBuildAnd(builder,lhs,rhs,"andtmp");
        case OR: return LLVMBuildOr(builder,lhs,rhs,"ortmp");
      }
    }

  }
  return NULL;
}


/**
 * @brief 
 * It takes a statement to generate code for it.
 * @param stmt is a statement.
 * @param module is a LLVMModuleRef.
 * @param builder is a LLVMBuilderRef.
 */
void codegen_stmt(struct stmt *stmt, LLVMModuleRef module, LLVMBuilderRef builder) {
  switch (stmt->type) {
    case STMT_SEQ: {
      codegen_stmt(stmt->seq.fst, module, builder);
      codegen_stmt(stmt->seq.snd, module, builder);
      break;
    }

    case STMT_ASSIGN: {
      LLVMValueRef expr = codegen_expr(stmt->assign.expr, module, builder);
      LLVMBuildStore(builder, expr, vector_get(&global_types, stmt->assign.id));
      break;
    }

    case STMT_PRINT: {
      enum value_type arg_type = check_types(stmt->print.expr);
      LLVMValueRef print_fn = LLVMGetNamedFunction(module, arg_type == BOOLEAN ? "print_i1" : "print_i32");
      LLVMValueRef args[] = { codegen_expr(stmt->print.expr, module, builder) };
      LLVMBuildCall(builder, print_fn, args, 1, "");  // It calles function by LLVMValueref with parameter
      break;
    }

    case STMT_WHILE: {
      LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
      LLVMBasicBlockRef cond_bb = LLVMAppendBasicBlock(func, "cond");
      LLVMBasicBlockRef body_bb = LLVMAppendBasicBlock(func, "body");
      LLVMBasicBlockRef cont_bb = LLVMAppendBasicBlock(func, "cont");

      LLVMBuildBr(builder, cond_bb);

      LLVMPositionBuilderAtEnd(builder, cond_bb);
      LLVMValueRef cond = codegen_expr(stmt->while_.cond, module, builder);
      LLVMBuildCondBr(builder, cond, body_bb, cont_bb);

      LLVMPositionBuilderAtEnd(builder, body_bb);
      codegen_stmt(stmt->while_.body, module, builder);
      LLVMBuildBr(builder, cond_bb);

      LLVMPositionBuilderAtEnd(builder, cont_bb);
      break;
    }

    case STMT_IF: {
      LLVMValueRef func = LLVMGetBasicBlockParent(LLVMGetInsertBlock(builder));
      LLVMBasicBlockRef body_bb = LLVMAppendBasicBlock(func, "body");
      LLVMBasicBlockRef else_bb = LLVMAppendBasicBlock(func, "else");
      LLVMBasicBlockRef cont_bb = LLVMAppendBasicBlock(func, "cont");

      LLVMValueRef cond = codegen_expr(stmt->ifelse.cond, module, builder);
      LLVMBuildCondBr(builder, cond, body_bb, else_bb);

      LLVMPositionBuilderAtEnd(builder, body_bb);
      codegen_stmt(stmt->ifelse.if_body, module, builder);
      LLVMBuildBr(builder, cont_bb);

      LLVMPositionBuilderAtEnd(builder, else_bb);
      if (stmt->ifelse.else_body) {
        codegen_stmt(stmt->ifelse.else_body, module, builder);
      }
      LLVMBuildBr(builder, cont_bb);

      LLVMPositionBuilderAtEnd(builder, cont_bb);
      break;
    }

    case STMT_INCREMENT:

      LLVMValueRef expr = codegen_expr(stmt->assign.expr, module, builder);
      LLVMBuildStore(builder, expr, vector_get(&global_types, stmt->assign.id));

      break;

    case STMT_DECREMENT:
      
      break;

  }
}
