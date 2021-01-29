#define	OP_ALU		1				/* arithmetic-logic-op  */
#define	OP_DEC		2				/* arithmetic-logic-op  */
#define	OP_INC		3				/* arithmetic-logic-op  */
#define	OP_LOAD		4				/* arithmetic-logic-op  */
#define	OP_STORE		5				/* arithmetic-logic-op  */
#define	OP_POP		6				/* arithmetic-logic-op  */
#define	OP_JUMPZ		7				/* arithmetic-logic-op  */
#define	OP_JUMP		8				/* arithmetic-logic-op  */
#define	OP_CALL		9				/* arithmetic-logic-op  */
#define	OP_ENTRY		10				/* arithmetic-logic-op  */
#define	OP_RETURN	11				/* arithmetic-logic-op  */

/*
 *		region modifies
 */
#define MOD_GLOBAL	 1				/* global region        */
#define MOD_PARAM	 	 2				/* parameter region 		*/
#define MOD_LOCAL 	 3				/* local region			*/
#define MOD_IMMED	 	 4				/* load only: constant  */



#define ALU_ADD		 1				/* addition					*/
#define ALU_SUB   	 2				/* SUBTRACTION	  			*/
#define ALU_MUL		 3				/* MULTIPLICATION			*/
#define ALU_DIV		 4				/* DIVISION					*/
#define ALU_MOD		 5				/* REMAINDER				*/
#define ALU_LT 		 6				/* COMPARES AS: <			*/
#define ALU_GT 		 7				/* addition		 >			*/
#define ALU_LE 		 8				/* addition		 <=		*/
#define ALU_GE 		 9				/* addition		 >=		*/
#define ALU_EQ 		10				/* addition		 ==		*/
#define ALU_NE 	 	11				/* addition		 !=		*/
#define ALU_AND		12				/* BITWISE AND				*/
#define ALU_OR 		13				/* BIT WISE OR				*/
#define ALU_XOR		14				/* BIT-WISE EXCL. OR		*/

/*
 * program memory structure
 */

struct prog {
		short p_op;				/* operation code */
		short	p_mod;			/* modifieer       */
		int	p_val				/* offest or other value */
		};

/*
 *		tunable limits
 */
#define	L_PROG	200			/* max. program size */
#define	L_DATA	100			/* max. area for stack,etc. */
#define DIM(x)		(sizeof x / sizeof x[0]) /* extent*/
