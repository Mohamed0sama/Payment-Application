/*
 * app.c
 *
 *  Created on: Oct 12, 2022
 *      Author: maham
 */

#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include "app.h"




void appstart(void) {
	
	

	//CARD 
	ST_cardData_t client; //structure client for card data

	getCardHolderName(&client);
	getCardExpiryDate(&client);
	getCardPAN(&client);

	//terminal
	ST_terminalData_t TerminalData;
	EN_terminalError_t Terminalstate;
	setMaxAmount(&TerminalData);

	Terminalstate = getTransactionDate(&TerminalData);

	if (Terminalstate == T_OK) {
		Terminalstate = isCardExpired(client, TerminalData);
		if (Terminalstate == EXPIRED_CARD) {
			printf("EXPIRED_CARD");
			return 0;
		}
		if (Terminalstate == T_OK) {
			getTransactionAmount(&TerminalData);
		}
		Terminalstate=isBelowMaxAmount(&TerminalData);
		if (Terminalstate == INVALID_MAX_AMOUNT) {
			printf("EXCEEDS MAXIMUM AMOUNT");
			return 0;
		}
	}

	//server
	ST_transaction_t TransactionData = { .cardHolderData = client,
										 .terminalData = TerminalData };

	EN_transState_t Transaction_state;

	Transaction_state=recieveTransactionData(&TransactionData);
	switch (Transaction_state) {
	case (FRAUD_CARD):
		printf("DECLINED\nINVALID ACCOUNT\n");
		return 0;
	case(DECLINED_STOLEN_CARD):
		printf("DECLINED STOLEN CARD\n");
		return 0;
	case(DECLINED_INSUFFECIENT_FUND):
		printf("DECLINED\nINSUFFECIENT_FUND\n");
		return 0;
	case(INTERNAL_SERVER_ERROR):
		printf("FAILED TO SAVE TRANSACTION\n");
		return 0;
	case(APPROVED):
		printf("SAVED TRANSACTION\n");
		return 0;
	default: return 0;
	}
}

int main() {
	setbuf(stdout, NULL);
	printf("\t^^^^^^^^^^^^^^^^^Payment Application^^^^^^^^^^^^^^^^^\n");
	appstart();
	return 0;
}
