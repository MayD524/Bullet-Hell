#include <bh_sighandler.h>

#ifdef _WIN32
#include <dbghelp.h>

void print_stack( void )
{
    unsigned int   i;
    void         * stack[ 100 ];
    unsigned short frames;
    SYMBOL_INFO  * symbol;
    HANDLE         process;

    process = GetCurrentProcess();

    SymInitialize( process, NULL, TRUE );

    frames               = CaptureStackBackTrace( 0, 100, stack, NULL );
    symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
    symbol->MaxNameLen   = 255;
    symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

    for( i = 0; i < frames; i++ )
    {
        SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );

        printf( "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address );
    }

    free( symbol );
}


LONG WINAPI exception_handler(EXCEPTION_POINTERS *ExceptionInfo) {
    print_stack();
    if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
        printf("Received access violation (segmentation fault).\n");
        printf("Address: %p\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
        // Attempt to print basic stack trace
        CONTEXT *context = ExceptionInfo->ContextRecord;
        #ifdef _M_X64
        printf("Instruction pointer (RIP): 0x%llx\n", context->Rip);
        #else
        printf("Instruction pointer (EIP): 0x%x\n", context->Eip);
        #endif
        exit(1);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

#endif

void signal_handler(int sig) {
    switch (sig) {
        case SIGINT:
            printf("Received SIGINT (Ctrl+C). Exiting gracefully.\n");
            // Perform cleanup if necessary
            exit(0);
            break;
        case SIGTERM:
            printf("Received SIGTERM. Exiting gracefully.\n");
            // Perform cleanup if necessary
            exit(0);
            break;
        case SIGSEGV:
            printf("Received SIGSEGV (Segmentation Fault).\n");
            // Perform cleanup if necessary
            exit(1);
            break;
        default:
            printf("Received signal %d. Ignoring.\n", sig);
            break;
    }
}

void initialize_signal_handler() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    #ifdef _WIN32
        SetUnhandledExceptionFilter(exception_handler);
    #else
        signal(SIGSEGV, signal_handler);
    #endif
}