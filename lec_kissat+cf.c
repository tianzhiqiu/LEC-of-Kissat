/**CFile****************************************************************

  FileName    [demo.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName [ABC as a static library.]

  Synopsis    [A demo program illustrating the use of ABC as a static library.]

  Author      [Alan Mishchenko]
  
  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: demo.c,v 1.00 2005/11/14 00:00:00 alanmi Exp $]

***********************************************************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

#if defined(ABC_NAMESPACE)
namespace ABC_NAMESPACE
{
#elif defined(__cplusplus)
extern "C"
{
#endif

// procedures to start and stop the ABC framework
// (should be called before and after the ABC procedures are called)
void   Abc_Start();
void   Abc_Stop();

// procedures to get the ABC framework and execute commands in it
typedef struct Abc_Frame_t_ Abc_Frame_t;

Abc_Frame_t * Abc_FrameGetGlobalFrame();
int    Cmd_CommandExecute( Abc_Frame_t * pAbc, const char * sCommand );

#if defined(ABC_NAMESPACE)
}
using namespace ABC_NAMESPACE;
#elif defined(__cplusplus)
}
#endif

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [The main() procedure.]

  Description [This procedure compiles into a stand-alone program for 
  DAG-aware rewriting of the AIGs. A BLIF or PLA file to be considered
  for rewriting should be given as a command-line argument. Implementation 
  of the rewriting is inspired by the paper: Per Bjesse, Arne Boralv, 
  "DAG-aware circuit compression for formal verification", Proc. ICCAD 2004.]
               
  SideEffects []

  SeeAlso     []

***********************************************************************/
int main( int argc, char * argv[] )
{
    // parameters
    int fPretreat = 1;
    // int fUseResyn2  = 0;
    // int fPrintStats = 1;
    // int fVerify     = 1;
    // variables
    Abc_Frame_t * pAbc;
    char * pFileName;
    char Command[1000];
    clock_t clkRead, clkPretreat, clkSat, clkAll, clk, clkStart;

    //////////////////////////////////////////////////////////////////////////
    // get the input file name
    if ( argc != 2 )
    {
        printf( "Wrong number of command-line arguments.\n" );
        return 1;
    }
    pFileName = argv[1];

    //////////////////////////////////////////////////////////////////////////
    // start the ABC framework
    Abc_Start();
    pAbc = Abc_FrameGetGlobalFrame();

clk = clock();
clkStart = clk;
    //////////////////////////////////////////////////////////////////////////
    // read the aiger file
    sprintf( Command, "read_aiger %s", pFileName );
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        return 1;
    }


clkRead = clock() - clk;

clk = clock();

    //////////////////////////////////////////////////////////////////////////
    // Pre-treatment
    if ( fPretreat )
    {
        sprintf( Command, "dchoice -l; drwsat");
        if ( Cmd_CommandExecute( pAbc, Command ) )
        {
            fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
            return 1;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // write the cnf file
    sprintf( Command, "write_cnf test.cnf");
    if ( Cmd_CommandExecute( pAbc, Command ) )
    {
        fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
        return 1;
    }
clkPretreat = clock() - clk;


clk = clock();

    //////////////////////////////////////////////////////////////////////////
    // sat
    sprintf( Command, "./kissat+cf test.cnf test_out" );
    system(Command);
    // if ( Cmd_CommandExecute( pAbc, Command ) )
    // {
    //     fprintf( stdout, "Cannot execute command \"%s\".\n", Command );
    //     return 1;
    // }

    
clkSat = clock() - clk;
clkAll = clock() - clkStart;


    printf( "Reading = %6.2f sec   ",     (float)(clkRead)/(float)(CLOCKS_PER_SEC) );
    printf( "Pre-treatment = %6.2f sec   ",   (float)(clkPretreat)/(float)(CLOCKS_PER_SEC) );
    printf( "SAT slover = %6.2f sec   ",   (float)(clkSat)/(float)(CLOCKS_PER_SEC) );
    printf( "All = %6.2f sec\n", (float)(clkAll)/(float)(CLOCKS_PER_SEC) );

    //////////////////////////////////////////////////////////////////////////
    // stop the ABC framework
    Abc_Stop();
    return 0;
}




