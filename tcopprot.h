/*-------------------------------------------------------------------------
 *
 * tcopprot.h
 *	  prototypes for postgres.c.
 *
 *
 * Portions Copyright (c) 1996-2018, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * src/include/tcop/tcopprot.h
 *
 * OLD COMMENTS
 *	  This file was created so that other c files could get the two
 *	  function prototypes without having to include tcop.h which single
 *	  handedly includes the whole f*cking tree -- mer 5 Nov. 1991
 *
 *-------------------------------------------------------------------------
 */
#ifndef TCOPPROT_H
#define TCOPPROT_H

#include "nodes/params.h"
#include "nodes/parsenodes.h"
#include "nodes/plannodes.h"
#include "storage/procsignal.h"
#include "utils/guc.h"
#include "utils/queryenvironment.h"


/* Required daylight between max_stack_depth and the kernel limit, in bytes */
#define STACK_DEPTH_SLOP (512 * 1024L)

extern CommandDest whereToSendOutput;
extern PGDLLIMPORT const char *debug_query_string;
extern int	max_stack_depth;
extern int	PostAuthDelay;

/* GUC-configurable parameters */

typedef enum
{
	LOGSTMT_NONE,				/* log no statements */
	LOGSTMT_DDL,				/* log data definition statements */
	LOGSTMT_MOD,				/* log modification statements, plus DDL */
	LOGSTMT_ALL					/* log all statements */
} LogStmtLevel;

extern PGDLLIMPORT int log_statement;

extern List *pg_parse_query(const char *query_string);
extern List *pg_analyze_and_rewrite(RawStmt *parsetree,
					   const char *query_string,
					   Oid *paramTypes, int numParams,
					   QueryEnvironment *queryEnv);
extern List *pg_analyze_and_rewrite_params(RawStmt *parsetree,
							  const char *query_string,
							  ParserSetupHook parserSetup,
							  void *parserSetupArg,
							  QueryEnvironment *queryEnv);
extern PlannedStmt *pg_plan_query(Query *querytree, int cursorOptions,
			  ParamListInfo boundParams);
extern List *pg_plan_queries(List *querytrees, int cursorOptions,
				ParamListInfo boundParams);

extern bool check_max_stack_depth(int *newval, void **extra, GucSource source);
extern void assign_max_stack_depth(int newval, void *extra);

extern void die(SIGNAL_ARGS);
extern void quickdie(SIGNAL_ARGS) pg_attribute_noreturn();
extern void StatementCancelHandler(SIGNAL_ARGS);
extern void FloatExceptionHandler(SIGNAL_ARGS) pg_attribute_noreturn();
extern void RecoveryConflictInterrupt(ProcSignalReason reason); /* called from SIGUSR1
																 * handler */
extern void ProcessClientReadInterrupt(bool blocked);
extern void ProcessClientWriteInterrupt(bool blocked);

extern void process_postgres_switches(int argc, char *argv[],
						  GucContext ctx, const char **dbname);
extern void PostgresMain(int argc, char *argv[],
			 const char *dbname,
			 const char *username) pg_attribute_noreturn();
extern long get_stack_depth_rlimit(void);
extern void ResetUsage(void);
extern void ShowUsage(const char *title);
extern int	check_log_duration(char *msec_str, bool was_logged);
extern void set_debug_options(int debug_flag,
				  GucContext context, GucSource source);
extern bool set_plan_disabling_options(const char *arg,
						   GucContext context, GucSource source);
extern const char *get_stats_option_name(const char *arg);

//add gaojt [Robust-Query-Optimization] 2019-02-15:b
#define MATRIX_LENGHT 10

extern void pg_general_prunning(Query *query);
extern List *pg_gen_transitive_closure(Query *query, List *tcs);
extern List *gen_transitive_closures(List *join_quals, List *tcs);
extern bool exclusive(void *a, void *b);
extern List* fill_vmi(List *vars, List *vmis);
extern void *gen_transitive_closure(List *join_quals, Bitmapset *local_pool, void* join_qual, void *tc);
extern void print_transitive_closures(List *tcs);
extern void print_vmis(List *vmis);
extern void print_pretty_range(void *range, const char *comment);
extern void print_range(int v_t, Datum l_b, Datum u_b, bool l_i, bool u_i, bool l_in, bool u_in, char *res);
extern void print_join_quals(Query *query);
extern void print_array(void *a, int length, const char *comment);
extern void pg_range_prunning(Query *query);
extern void apply_pus(List *pus, Query *query);
extern void apply_pu(void *pu, Query *query);
extern void *apply_one_range(void *tci_r, Node *jtnode, bool *is_matched);
extern bool match_and_modify_expr(void *tci_r, void *expr, void *bool_and_expr);
extern List *range_prunning(Query *query, List *tcs);
extern List *gen_prune_units_for_tcs(Query *query, List *tcs, List *pus);
extern void* align_prune_units(Query *query, List *pus);
extern int get_max_align_times(void *prune_unit);
extern List *walk_and_visit_quals(Node *node, List *prs);
extern List *gen_prune_units(List *tcs, List *prs, List *pus);
extern bool examine_pu(void *prune_unit);
extern void *gen_prune_unit(void *tc, List *prs, void *pu);
extern void gen_prunning_range(void *tci, List *prs, void *tr);
extern bool is_column_const_form(void *pr);
extern bool include(int v, int a[], int a_len);
extern bool match (void *v, void *m);
extern void gen_range(void *tci, void *pr, void *tr);
extern void update_range(int opno, void *con, void *tr);
extern void update_int4_range(int opno, void *con, void *tr);
extern List *gen_equivalence_class(List *gecs, Query *query);
extern void LIP_prunning(Node* gec, Query* query);
extern List *pg_gen_coded_join_quals(Node *jtnode, List *join_quals);
extern List *gen_coded_join_quals(Var *l_var, Var *r_var, List *vars);
extern bool pg_check_var_existing(Var *l_var, Var *r_var ,List *vars);
extern List *pg_fill_vars(Var *l_var, Var *r_var, List *vars);
extern void pg_fill_M(void *jq, int M[][MATRIX_LENGHT]);
extern int assign_max_index(List *vars);
extern void assign_index_for_jq(List *vars, void *jq);
extern void pg_warshall(int M[][MATRIX_LENGHT]);
extern List* gen_2tci_by_vmis(int a, int b, List *vmis, List *tc);
extern bool is_tci_exist(void *vmi, List *tc);
extern void* align_prune_unit(void *prune_unit, bool is_sub_range, bool *is_valid_father_range, bool *is_finish_aligned, void *new_com_range);
extern bool is_available_range(void *range);
extern void construct_VTMapVV(void *prune_unit, bool is_sub_range, void *left_bounds, void *right_bounds);
extern void* get_com_range(void *left_bounds, void *right_bounds, int length);
extern void* apply_cr(void *com_range, List *sub_ranges);
extern void* apply_cr_father_range(void *com_r, void *pu);
extern void apply_cr_children_ranges(void *com_range, void *prune_unit, bool *is_total_devoured);
extern bool is_legal_range(void *com_range);
extern void refine_prune_unit(Query *query, List *tci_rs);
extern void* select_split_strategy(void *tci_r);
extern void  eat_data(Query *query, void *tci_r);
extern void align_HR(void *range,void *hrs,int hr_len,int *lower_location,int *upper_location);
extern void after_eat(void *tci_r, void *hr_values, int nvalues);
extern void eat(void *tci_r, Datum lower_bound, Datum upper_bound, bool *eat_finshed);
extern void get_hr_values(Oid relid, int varattno, void *hslot);
extern void* even_split_range(void *o_r);
extern int   compare(const void *a, const void *b);
extern int   com_datum(const void *a, const void *b);
extern int   com_int4_datum(const void *a, const void *b);

//add gaojt 2019-02-15:e

#endif							/* TCOPPROT_H */
