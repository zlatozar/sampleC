#define	OP_ALU		"alu"				/* arithmetic-logic-op  */
#define	OP_DEC		"dec"				/* arithmetic-logic-op  */
#define	OP_INC		"inc"				/* arithmetic-logic-op  */
#define	OP_LOAD		"load"				/* arithmetic-logic-op  */
#define	OP_STORE		"store"				/* arithmetic-logic-op  */
#define	OP_POP		"pop"				/* arithmetic-logic-op  */
#define	OP_JUMPZ		"jumpz"				/* arithmetic-logic-op  */
#define	OP_JUMP		"jump"				/* arithmetic-logic-op  */
#define	OP_CALL		"call"				/* arithmetic-logic-op  */
#define	OP_ENTRY		"entry"				/* arithmetic-logic-op  */
#define	OP_RETURN	"return"				/* arithmetic-logic-op  */

/*
 *		region modifies
 */
#define MOD_GLOBAL "gbl"				/* global region        */
#define MOD_PARAM	 "par"				/* parameter region 		*/
#define MOD_LOCAL  "lcl"				/* local region			*/
#define MOD_IMMED	 "con"				/* load only: constant  */



#define ALU_ADD		"+"				/* addition					*/
#define ALU_SUB   	"-"				/* SUBTRACTION	  			*/
#define ALU_MUL		"*"				/* MULTIPLICATION			*/
#define ALU_DIV		"/"				/* DIVISION					*/
#define ALU_MOD		"%"				/* REMAINDER				*/
#define ALU_LT 		"<"				/* COMPARES AS: <			*/
#define ALU_GT 		">"				/* addition		 >			*/
#define ALU_LE 		"<="				/* addition		 <=		*/
#define ALU_GE 		">="				/* addition		 >=		*/
#define ALU_EQ 		"=="				/* addition		 ==		*/
#define ALU_NE 	 	"!="				/* addition		 !=		*/
#define ALU_AND		"&"				/* BITWISE AND				*/
#define ALU_OR 		"|"				/* BIT WISE OR				*/
#define ALU_XOR		"^"				/* BIT-WISE EXCL. OR		*/

/*
 *		tPED FUNCTION AND CODE GENERATOR
 */
char * gen_mod();			/* region modifier */
