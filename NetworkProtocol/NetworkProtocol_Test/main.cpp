
#include <DataTransferObjects/bestorder_test.h>
#include <DataTransferObjects/lasttransaction_test.h>
#include <DataTransferObjects/order_test.h>

#include <Responses/failuremsg_test.h>
#include <Responses/okmsg_test.h>
#include <Responses/registerusersuccessmsg_test.h>
#include <Responses/lasttransactionmsg_test.h>
#include <Responses/listofordersmsg_test.h>
#include <Responses/listofstocksmsg_test.h>
#include <Responses/orderacceptedmsg_test.h>
#include <Responses/orderchangemsg_test.h>
#include <Responses/ordercompletedmsg_test.h>
#include <Responses/showbestordermsg_test.h>
#include <Responses/shownewordermsg_test.h>
#include <Responses/shownobestordermsg_test.h>
#include <Responses/stockinfomsg_test.h>


#include <Requests/buystockmsg_test.h>
#include <Requests/sellstockmsg_test.h>
#include <Requests/cancelordermsg_test.h>
#include <Requests/getmyordersmsg_test.h>
#include <Requests/getmystocksmsg_test.h>
#include <Requests/getstockinfomsg_test.h>
#include <Requests/subscribestockmsg_test.h>
#include <Requests/unsubscribestockmsg_test.h>
#include <Requests/loginusermsg_test.h>
#include <Requests/registerusermsg_test.h>

#include <networkprotocol_utilities_test.h>

/*
 *
 *  About methodology
 *      assert for is being used for asserting things about testing (not tested!) code -
 *      if assert doesn't hold then there is bug in test code.
 *
 *      QVERIFY2, QFAIL, etc. is being used for asserting some traits of tested code -
 *      if that fail then there is bug in tested code, and unit test proved its value.
 *
 */


int main(int argc, char *argv[])
{
    bool result = 0;

    //              DTO
    BestOrderTest       bestOrder_test;
    LastTransactionTest lastTransaction_test;
    OrderTest           order_test;

    result |= QTest::qExec(&bestOrder_test, argc, argv);
    result |= QTest::qExec(&lastTransaction_test, argc, argv);
    result |= QTest::qExec(&order_test, argc, argv);

    // Responses
    FailureTest                 failure_test;
    OkTest                      ok_test;
    RegisterUserSuccessTest     register_user_success_test;
    LastTransactionResponseTest last_transaction_response_test;
    ListOfOrdersTest            list_of_orders_test;
    ListOfStocksTest            list_of_stocks_test;
    OrderAcceptedTest           order_accepted_test;
    OrderChangeTest             order_change_test;
    OrderCompletedTest          order_completed_test;
    ShowBestOrderTest           show_best_order_test;
    ShowNewOrderTest            show_new_order_test;
    ShowNoBestOrderTest         show_no_best_order_test;
    StockInfoTest               stock_info_test;

    result |= QTest::qExec(&failure_test, argc, argv);
    result |= QTest::qExec(&ok_test, argc, argv);
    result |= QTest::qExec(&register_user_success_test, argc, argv);
    result |= QTest::qExec(&last_transaction_response_test, argc, argv);
    result |= QTest::qExec(&list_of_orders_test, argc, argv);
    result |= QTest::qExec(&list_of_stocks_test, argc, argv);
    result |= QTest::qExec(&order_accepted_test, argc, argv);
    result |= QTest::qExec(&order_change_test, argc, argv);
    result |= QTest::qExec(&order_completed_test, argc, argv);
    result |= QTest::qExec(&show_best_order_test, argc, argv);
    result |= QTest::qExec(&show_new_order_test, argc, argv);
    result |= QTest::qExec(&show_no_best_order_test, argc, argv);
    result |= QTest::qExec(&stock_info_test, argc, argv);

    // Requests
    BuyStockTest                buy_stock_test;
    SellStockTest               sell_stock_test;
    CancelOrderTest             cancel_order_test;
    GetMyOrdersTest             get_my_orders_test;
    GetMyStocksTest             get_my_stocks_test;
    GetStockInfoTest            get_stock_info;
    SubscribeStockTest          subscribe_stock_test;
    UnsubscribeStockTest        unsubscribe_stock_test;
    LoginUserTest               login_user_test;
    RegisterUserTest            register_user_test;

    result |= QTest::qExec(&buy_stock_test, argc, argv);
    result |= QTest::qExec(&sell_stock_test, argc, argv);
    result |= QTest::qExec(&cancel_order_test, argc, argv);
    result |= QTest::qExec(&get_my_orders_test, argc, argv);
    result |= QTest::qExec(&get_my_stocks_test, argc, argv);
    result |= QTest::qExec(&get_stock_info, argc, argv);
    result |= QTest::qExec(&subscribe_stock_test, argc, argv);
    result |= QTest::qExec(&unsubscribe_stock_test, argc, argv);
    result |= QTest::qExec(&login_user_test, argc, argv);
    result |= QTest::qExec(&register_user_test, argc, argv);


    NetworkProtocolUtilitiesTest  network_protocol_utils;

    result |= QTest::qExec(&network_protocol_utils, argc, argv);

    return result;
}



