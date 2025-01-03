
/*
	hedging-test.c

	Test the market history prices using random hedging strategy 
	(long & short at the same time).


	Result: 
		- FAIL !
		- Forex is a example of a martingale. 
			 It can model with the possibility of bankruptcy.
*/


#include <stdio.h>
#include <windows.h>
#include <msxml6.h>
#include <time.h>

#pragma comment(lib, "msxml6.lib")


#define TAKEPROFIT 0.0010f		// 10 pips
#define STOPLOSS	  0.1000f		// 1000 pips


void main () {
	IXMLDOMDocument *doc;
	BSTR bstr;
	VARIANT_BOOL isSuccessful;
	IXMLDOMNodeList *rootNodeList;
	IXMLDOMNode *childNode;
	long list_length;
	int i;
	BOOL long_status = FALSE, short_status = FALSE;
	double long_trade, short_trade;
	long win = 0;
	VARIANT varFileName;
	BOOL bankrupt = FALSE;
	

	srand(time(NULL));

	CoInitialize(NULL);
	CoCreateInstance(&CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, &IID_IXMLDOMDocument, &doc);
	

	bstr = SysAllocString(L"EURUSD.xml");
	VariantInit(&varFileName);
	varFileName.vt = VT_BSTR;
	varFileName.bstrVal = bstr;
	
	doc->lpVtbl->load(doc, varFileName, &isSuccessful);			// load from file
	VariantClear(&varFileName);

	SysReAllocString(&bstr, L"price");
	doc->lpVtbl->getElementsByTagName(doc, bstr, &rootNodeList);
	SysFreeString(bstr);

	rootNodeList->lpVtbl->get_length(rootNodeList, &list_length);
	//wprintf(L"list_length = %d\n", list_length);
	
	for (i=0; i<list_length; i++) {
		rootNodeList->lpVtbl->nextNode(rootNodeList, &childNode);
		childNode->lpVtbl->get_text(childNode, &bstr);
		if (long_status == FALSE && short_status == FALSE) {
			long_trade = _wtof(bstr);
			short_trade = _wtof(bstr);
			long_status = TRUE;
			short_status = TRUE;
		} else {
			if (long_status == TRUE) {
				if (_wtof(bstr) <= (long_trade - STOPLOSS)) {
					bankrupt = TRUE;
					puts("bankrupt !!!");
					wprintf(L"current: %f vs long: %f\n", _wtof(bstr), long_trade);
					break;
				} else if (_wtof(bstr) >= (long_trade + TAKEPROFIT)) {
					win++;
					long_status = FALSE;
				}
			} else if (short_status == TRUE) {
				if (_wtof(bstr) >= (short_trade + STOPLOSS)) {
					bankrupt = TRUE;
					puts("bankrupt !!!");
					wprintf(L"current: %f vs short: %f\n", _wtof(bstr), short_trade);
					break;
				} else if (_wtof(bstr) <= (short_trade - TAKEPROFIT)) {
					win++;
					short_status = FALSE;
				}
			}
		}
		
		


		//wprintf(L"bstr = %s\n", bstr);
	}

	

	printf("win: %d\n", win);

	doc->lpVtbl->Release(doc);
	CoUninitialize();


	//puts("test 123");
	system("pause");
}

