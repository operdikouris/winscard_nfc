#include<winscard.h>
#include<stdio.h>

int main (void)
{
    SCARDCONTEXT    hSC;
    LONG            lReturn;
    LPTSTR          pmszReaders = NULL;
    LPTSTR          pReader;

    char         readerName[50];
    DWORD           cch = SCARD_AUTOALLOCATE;
    SCARDHANDLE     shanle;
    // LPSCARDHANDLE   phCard;
    DWORD           pdwActiveProtocol;
    // Establish the context.
    lReturn = SCardEstablishContext(SCARD_SCOPE_USER,
                                    NULL,
                                    NULL,
                                    &hSC);
    if ( SCARD_S_SUCCESS != lReturn ) {
        printf("Failed SCardEstablishContext\n");
        return 1;
    }

    

    // Retrieve the list the readers.
    // hSC was set by a previous call to SCardEstablishContext.
    lReturn = SCardListReaders(hSC,
                           NULL,
                           (LPTSTR)&pmszReaders,
                           &cch );
    if (lReturn == SCARD_S_SUCCESS )
    {
        pReader = pmszReaders;

        
        while ( '\0' != *pReader )
        {
            // Display the value.
            printf("Reader: %s\n", pReader );
            memset(readerName, 0, sizeof(readerName));
            memcpy(readerName, pReader, strlen(pReader));
            // Advance to the next value.
            pReader = pReader + strlen((char *)pReader) + 1;
        }
        
        
        for(;;){
        //select and connect the last reader found
        lReturn = SCardConnect(hSC, //context
                               readerName, //name
                               SCARD_SHARE_EXCLUSIVE, //shared mode 
                               SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, //preferred protocols
                               &shanle, //handle pointer
                               &pdwActiveProtocol //selected protocol
                               );
        if (lReturn == SCARD_S_SUCCESS)
            break;
        //SCARD_W_REMOVED_CARD
        }
        //Start sendind APDU commands
        //1. retrive fw version of acr122u reader
        const BYTE get_fw_command[] =
        {
            0xff, //Class
            0x00, //Ins
            0x48, //P1
            0x00, //P2
            0x00  //Le
        };
        BYTE response[256];
        DWORD length = sizeof(response);
        LONG rc;


        memset(response, 0, sizeof(0));
        lReturn = SCardTransmit(shanle, //handle
                                // (LPCSCARD_IO_REQUEST) pdwActiveProtocol, //protocol(returned above)
                                NULL,
                                get_fw_command, 
                                sizeof(get_fw_command), 
                                NULL,
                                response, 
                                &length);

        if (lReturn != SCARD_S_SUCCESS) {
            printf("SCardTransmit error\n");
            return 1;
        }
        if (length < 2){
            /* ... response is not correctly formatted (no SW1 SW2) – exit here */
            printf("Bad formati response\n");
            return 2;
        }

        printf("Firmware Version=");
        for (int i=0; i<length-2; i++)
        printf("%c", (char)response[i]);
        printf("\n");

        // Free the memory.
        lReturn = SCardFreeMemory( hSC,
                                   pmszReaders );
        if ( SCARD_S_SUCCESS != lReturn ){
            printf("Failed SCardFreeMemory\n");
            return 1;
        }

    }
    else 
    {
        printf("SCardListReaders returned: 0x%08x\n", lReturn);
    }

    //release context
    if (SCardReleaseContext(hSC) != SCARD_S_SUCCESS) {
        printf("Error releasing context\n");
        return 1;
    }
       
    return 0;
}