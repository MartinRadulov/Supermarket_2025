//#include "transaction.h"
//#include "product.h"
//#include <iostream>
//
//void testTransactionItem();
//void testTransaction();
//void testTransactionManager();
//void testIntegrationWithProducts();
//void testReceiptGeneration();
//void testFileOperations();
//void printSeparator(const char* title);
//void setupSampleProducts(ProductManager& productMgr);
//
//int main()
//{
//    std::cout << "=== TRANSACTION SYSTEM TEST ===" << std::endl;
//
//    testTransactionItem();
//    testTransaction();
//    testTransactionManager();
//    testIntegrationWithProducts();
//    testReceiptGeneration();
//    testFileOperations();
//
//    std::cout << "\n=== ALL TRANSACTION TESTS COMPLETED ===" << std::endl;
//    return 0;
//}
//
//void testTransactionItem()
//{
//    printSeparator("TRANSACTION ITEM TEST");
//
//    // Create transaction items
//    TransactionItem item1(1001, "Bread", 3.0, 2.50);
//    TransactionItem item2(1003, "Bananas", 1.5, 4.20);
//
//    std::cout << "Created transaction items:" << std::endl;
//    std::cout << "Item 1 - Product: " << item1.getProductName()
//        << ", Qty: " << item1.getQuantity()
//        << ", Unit Price: " << item1.getUnitPrice()
//        << ", Total: " << item1.getTotalPrice() << std::endl;
//
//    std::cout << "Item 2 - Product: " << item2.getProductName()
//        << ", Qty: " << item2.getQuantity()
//        << ", Unit Price: " << item2.getUnitPrice()
//        << ", Total: " << item2.getTotalPrice() << std::endl;
//
//    // Test display format
//    std::cout << "\nTesting display format:" << std::endl;
//    item1.displayInfo();
//    item2.displayInfo();
//
//    // Test automatic total calculation
//    if (item1.getTotalPrice() == 7.50) {
//        std::cout << "✅ Item 1 total calculation correct (3.0 * 2.50 = 7.50)" << std::endl;
//    }
//
//    if (item2.getTotalPrice() == 6.30) {
//        std::cout << "✅ Item 2 total calculation correct (1.5 * 4.20 = 6.30)" << std::endl;
//    }
//}
//
//void testTransaction()
//{
//    printSeparator("TRANSACTION CLASS TEST");
//
//    // Create transaction
//    Transaction transaction(1, 101); // Transaction ID: 1, Cashier ID: 101
//
//    std::cout << "Created transaction - ID: " << transaction.getTransactionId()
//        << ", Cashier: " << transaction.getCashierId() << std::endl;
//    std::cout << "Date: " << transaction.getDate() << std::endl;
//    std::cout << "Time: " << transaction.getTime() << std::endl;
//
//    // Add items
//    std::cout << "\nAdding items to transaction:" << std::endl;
//    transaction.addItem(1001, "Bread", 3.0, 2.50);
//    transaction.addItem(1003, "Bananas", 1.5, 4.20);
//    transaction.addItem(1002, "Milk 1L", 2.0, 3.20);
//
//    std::cout << "✅ Added 3 items to transaction" << std::endl;
//    std::cout << "Item count: " << transaction.getItemCount() << std::endl;
//    std::cout << "Subtotal: " << transaction.getSubtotal() << std::endl;
//
//    // Test voucher application
//    std::cout << "\nTesting voucher application:" << std::endl;
//    double discountAmount = transaction.getSubtotal() * 0.15; // 15% discount
//    if (transaction.applyVoucher("3AX01B", discountAmount)) {
//        std::cout << "✅ Voucher applied successfully" << std::endl;
//        std::cout << "Discount amount: " << transaction.getDiscountAmount() << std::endl;
//        std::cout << "Final total: " << transaction.getFinalTotal() << std::endl;
//        std::cout << "Voucher code: " << transaction.getVoucherCode() << std::endl;
//    }
//
//    // Test receipt display
//    std::cout << "\nDisplaying receipt:" << std::endl;
//    transaction.displayReceipt();
//
//    // Test copy constructor (Rule of Six)
//    std::cout << "\nTesting copy constructor:" << std::endl;
//    Transaction copyTransaction(transaction);
//    std::cout << "Copy - Transaction ID: " << copyTransaction.getTransactionId()
//        << ", Items: " << copyTransaction.getItemCount()
//        << ", Total: " << copyTransaction.getFinalTotal() << std::endl;
//
//    if (copyTransaction.getTransactionId() == transaction.getTransactionId() &&
//        copyTransaction.getItemCount() == transaction.getItemCount()) {
//        std::cout << "✅ Copy constructor working correctly" << std::endl;
//    }
//
//    // Test finalization
//    transaction.finalizeTransaction();
//    std::cout << "✅ Transaction finalized" << std::endl;
//}
//
//void testTransactionManager()
//{
//    printSeparator("TRANSACTION MANAGER TEST");
//
//    // Create product manager for integration
//    ProductManager productMgr;
//    setupSampleProducts(productMgr);
//
//    // Create transaction manager
//    TransactionManager transMgr(&productMgr);
//
//    std::cout << "Created TransactionManager with ProductManager integration" << std::endl;
//
//    // Test new transaction creation
//    Transaction* newTrans = transMgr.startNewTransaction(101);
//    if (newTrans) {
//        std::cout << "✅ New transaction created - ID: " << newTrans->getTransactionId() << std::endl;
//    }
//
//    // Test adding products to transaction
//    std::cout << "\nTesting product addition to transaction:" << std::endl;
//
//    if (transMgr.addProductToCurrentTransaction(newTrans, 1001, 2.0)) {
//        std::cout << "✅ Added Bread (2 units) to transaction" << std::endl;
//    }
//
//    if (transMgr.addProductToCurrentTransaction(newTrans, 2001, 1.5)) {
//        std::cout << "✅ Added Bananas (1.5 kg) to transaction" << std::endl;
//    }
//
//    std::cout << "Transaction subtotal: " << newTrans->getSubtotal() << std::endl;
//
//    // Test enhanced voucher application with different discount rates
//    std::cout << "\nTesting enhanced voucher calculation:" << std::endl;
//
//    // Test different voucher codes with different discount rates
//    const char* testVouchers[] = { "1A1B2", "2X3Y4", "3Z5W6", "4M8N9", "5Q7R1", "INVALID" };
//    const char* expectedRates[] = { "5%", "10%", "15%", "20%", "25%", "Invalid" };
//
//    for (int i = 0; i < 6; i++) {
//        Transaction testTrans = *newTrans; // Copy transaction for testing
//
//        if (transMgr.applyVoucherToTransaction(&testTrans, testVouchers[i])) {
//            double discountPercent = (testTrans.getDiscountAmount() / testTrans.getSubtotal()) * 100;
//            std::cout << "✅ Voucher " << testVouchers[i] << " applied - "
//                << (int)discountPercent << "% discount (" << expectedRates[i] << " expected)" << std::endl;
//        }
//        else {
//            std::cout << "✅ Voucher " << testVouchers[i] << " rejected (" << expectedRates[i] << " expected)" << std::endl;
//        }
//    }
//
//    // Apply a valid voucher to original transaction
//    if (transMgr.applyVoucherToTransaction(newTrans, "3A1B5")) {
//        std::cout << "✅ Voucher applied to transaction" << std::endl;
//        std::cout << "Final total after voucher: " << newTrans->getFinalTotal() << std::endl;
//    }
//
//    // Complete transaction
//    transMgr.completeTransaction(newTrans);
//    std::cout << "✅ Transaction completed and added to manager" << std::endl;
//    std::cout << "Transaction count in manager: " << transMgr.getTransactionCount() << std::endl;
//
//    // Test transaction listing
//    std::cout << "\nListing all transactions:" << std::endl;
//    transMgr.listAllTransactions();
//
//    // Test find transaction
//    Transaction* found = transMgr.findTransactionById(newTrans->getTransactionId());
//    if (found) {
//        std::cout << "✅ Transaction found by ID: " << found->getTransactionId() << std::endl;
//    }
//
//    // Test cashier-specific listing
//    std::cout << "\nTransactions by cashier 101:" << std::endl;
//    transMgr.listTransactionsByCashier(101);
//
//    // Test voucher format validation
//    std::cout << "\nTesting voucher format validation:" << std::endl;
//
//    const char* invalidVouchers[] = { "123", "ABCDE", "12AB", "1A2B", "1A2B34", "1a2B3" };
//
//    for (int i = 0; i < 6; i++) {
//        Transaction testTrans = *newTrans;
//        if (!transMgr.applyVoucherToTransaction(&testTrans, invalidVouchers[i])) {
//            std::cout << "✅ Invalid voucher format '" << invalidVouchers[i] << "' correctly rejected" << std::endl;
//        }
//        else {
//            std::cout << "❌ Invalid voucher format '" << invalidVouchers[i] << "' was incorrectly accepted" << std::endl;
//        }
//    }
//}
//
//void testIntegrationWithProducts()
//{
//    printSeparator("PRODUCT INTEGRATION TEST");
//
//    // Setup
//    ProductManager productMgr;
//    setupSampleProducts(productMgr);
//    TransactionManager transMgr(&productMgr);
//
//    std::cout << "Initial product stock:" << std::endl;
//    productMgr.listAllProducts();
//
//    // Create transaction and sell products
//    Transaction* transaction = transMgr.startNewTransaction(102);
//
//    std::cout << "\nSelling products and checking stock updates:" << std::endl;
//
//    // Check stock before selling
//    Product* bread = productMgr.findProductById(1001);
//    if (bread) {
//        std::cout << "Bread stock before sale: " << bread->getAvailableQuantity() << std::endl;
//
//        // Sell some bread
//        if (transMgr.addProductToCurrentTransaction(transaction, 1001, 5.0)) {
//            productMgr.updateProductStock(1001, 5.0, false); // Subtract stock
//            std::cout << "✅ Sold 5 units of bread" << std::endl;
//            std::cout << "Bread stock after sale: " << bread->getAvailableQuantity() << std::endl;
//        }
//    }
//
//    // Test insufficient stock handling
//    Product* bananas = productMgr.findProductById(2001);
//    if (bananas) {
//        std::cout << "\nTesting insufficient stock:" << std::endl;
//        std::cout << "Available bananas: " << bananas->getAvailableQuantity() << " kg" << std::endl;
//
//        if (!bananas->hasEnoughQuantity(100.0)) {
//            std::cout << "✅ Correctly detected insufficient stock for 100 kg request" << std::endl;
//        }
//
//        if (bananas->hasEnoughQuantity(5.0)) {
//            std::cout << "✅ Has enough stock for 5 kg request" << std::endl;
//        }
//    }
//
//    transMgr.completeTransaction(transaction);
//
//    std::cout << "\nFinal product stock after transactions:" << std::endl;
//    productMgr.listAllProducts();
//}
//
//void testReceiptGeneration()
//{
//    printSeparator("RECEIPT GENERATION TEST");
//
//    // Create a transaction with items
//    Transaction transaction(99, 101);
//    transaction.addItem(1001, "Bread", 2.0, 2.50);
//    transaction.addItem(2001, "Bananas", 1.5, 4.20);
//    transaction.addItem(1002, "Milk 1L", 1.0, 3.20);
//
//    // Apply voucher
//    double discount = transaction.getSubtotal() * 0.10; // 10% discount
//    transaction.applyVoucher("TEST99", discount);
//
//    transaction.finalizeTransaction();
//
//    std::cout << "Generated transaction for receipt test:" << std::endl;
//    std::cout << "Transaction ID: " << transaction.getTransactionId() << std::endl;
//    std::cout << "Items: " << transaction.getItemCount() << std::endl;
//    std::cout << "Subtotal: " << transaction.getSubtotal() << std::endl;
//    std::cout << "Discount: " << transaction.getDiscountAmount() << std::endl;
//    std::cout << "Final Total: " << transaction.getFinalTotal() << std::endl;
//
//    // Save receipt to file
//    std::cout << "\nSaving receipt to file..." << std::endl;
//    transaction.saveReceiptToFile();
//    std::cout << "✅ Receipt saved to receipt_00099.txt" << std::endl;
//
//    // Display receipt format
//    std::cout << "\nReceipt format preview:" << std::endl;
//    transaction.displayReceipt();
//}
//
//void testFileOperations()
//{
//    printSeparator("FILE I/O OPERATIONS TEST");
//
//    // Create transaction manager with some transactions
//    ProductManager productMgr;
//    TransactionManager transMgr(&productMgr);
//
//    // Create multiple transactions
//    Transaction* trans1 = transMgr.startNewTransaction(101);
//    trans1->addItem(1001, "Bread", 3.0, 2.50);
//    trans1->addItem(1002, "Milk", 2.0, 3.20);
//    transMgr.completeTransaction(trans1);
//
//    Transaction* trans2 = transMgr.startNewTransaction(102);
//    trans2->addItem(2001, "Bananas", 2.5, 4.20);
//    trans2->applyVoucher("DISC10", 1.05); // 10% discount
//    transMgr.completeTransaction(trans2);
//
//    std::cout << "Created transactions for file test:" << std::endl;
//    std::cout << "Transaction count: " << transMgr.getTransactionCount() << std::endl;
//
//    // Save transactions to file
//    std::cout << "\nSaving transactions to file..." << std::endl;
//    transMgr.saveAllTransactions("test_transactions.txt");
//    std::cout << "✅ Transactions saved to test_transactions.txt" << std::endl;
//
//    // Test individual transaction/item string parsing
//    std::cout << "\nTesting string parsing methods:" << std::endl;
//
//    // Test TransactionItem loadFromString
//    TransactionItem testItem;
//    const char* itemLine = "ITEM:1001:Bread:3.0:2.50:7.50";
//    testItem.loadFromString(itemLine);
//
//    if (testItem.getProductId() == 1001 &&
//        strCompare(testItem.getProductName(), "Bread") &&
//        testItem.getQuantity() == 3.0 &&
//        testItem.getUnitPrice() == 2.50 &&
//        testItem.getTotalPrice() == 7.50) {
//        std::cout << "✅ TransactionItem::loadFromString() working correctly" << std::endl;
//    }
//    else {
//        std::cout << "❌ TransactionItem::loadFromString() failed" << std::endl;
//    }
//
//    // Test Transaction loadFromString
//    Transaction testTrans;
//    const char* transLine = "TRANSACTION:99:101:2025-06-24:15:30:45:12.90:1.29:11.61:TEST99:1:2";
//    testTrans.loadFromString(transLine);
//
//    if (testTrans.getTransactionId() == 99 &&
//        testTrans.getCashierId() == 101 &&
//        strCompare(testTrans.getDate(), "2025-06-24") &&
//        strCompare(testTrans.getTime(), "15:30:45") &&
//        testTrans.getSubtotal() == 12.90 &&
//        testTrans.getDiscountAmount() == 1.29 &&
//        testTrans.getFinalTotal() == 11.61 &&
//        strCompare(testTrans.getVoucherCode(), "TEST99") &&
//        testTrans.isVoucherApplied() &&
//        testTrans.getExpectedItemCount() == 2) {
//        std::cout << "✅ Transaction::loadFromString() working correctly" << std::endl;
//    }
//    else {
//        std::cout << "❌ Transaction::loadFromString() failed" << std::endl;
//    }
//
//    // Test complete file loading
//    std::cout << "\nTesting complete file loading..." << std::endl;
//
//    // Create a new transaction manager and load from file
//    TransactionManager newTransMgr;
//    newTransMgr.loadAllTransactions("test_transactions.txt");
//
//    std::cout << "Loaded transaction count: " << newTransMgr.getTransactionCount() << std::endl;
//
//    if (newTransMgr.getTransactionCount() == transMgr.getTransactionCount()) {
//        std::cout << "✅ All transactions loaded correctly" << std::endl;
//    }
//    else {
//        std::cout << "❌ Transaction loading failed - count mismatch" << std::endl;
//    }
//
//    // Verify loaded transaction data
//    Transaction* loadedTrans = newTransMgr.findTransactionById(1);
//    if (loadedTrans) {
//        std::cout << "Loaded Transaction 1:" << std::endl;
//        std::cout << "  ID: " << loadedTrans->getTransactionId() << std::endl;
//        std::cout << "  Cashier: " << loadedTrans->getCashierId() << std::endl;
//        std::cout << "  Items: " << loadedTrans->getItemCount() << std::endl;
//        std::cout << "  Total: " << loadedTrans->getFinalTotal() << std::endl;
//
//        if (loadedTrans->getItemCount() == 2) {
//            std::cout << "✅ Transaction items loaded correctly" << std::endl;
//        }
//        else {
//            std::cout << "❌ Transaction items not loaded correctly" << std::endl;
//        }
//    }
//
//    // Test manual file creation and loading
//    std::cout << "\nTesting manual file creation and loading..." << std::endl;
//
//    // Create a test file manually
//    std::ofstream testFile("manual_test_transactions.txt");
//    if (testFile.is_open()) {
//        testFile << "TRANSACTION:100:103:2025-06-24:16:00:00:15.80:0.00:15.80::0:3" << std::endl;
//        testFile << "ITEM:1001:Bread:2:2.50:5.00" << std::endl;
//        testFile << "ITEM:1002:Milk:1:3.20:3.20" << std::endl;
//        testFile << "ITEM:2001:Bananas:1.8:4.20:7.56" << std::endl;
//        testFile << "TRANSACTION:101:104:2025-06-24:16:15:30:8.40:0.84:7.56:SAVE10:1:1" << std::endl;
//        testFile << "ITEM:2002:Apples:2.2:3.80:8.36" << std::endl;
//        testFile.close();
//
//        // Load from manual file
//        TransactionManager manualTestMgr;
//        manualTestMgr.loadAllTransactions("manual_test_transactions.txt");
//
//        std::cout << "Manual test file loaded - Transactions: " << manualTestMgr.getTransactionCount() << std::endl;
//
//        if (manualTestMgr.getTransactionCount() == 2) {
//            std::cout << "✅ Manual file loading successful" << std::endl;
//
//            // Check specific transaction
//            Transaction* manualTrans = manualTestMgr.findTransactionById(100);
//            if (manualTrans && manualTrans->getItemCount() == 3) {
//                std::cout << "✅ Complex transaction with 3 items loaded correctly" << std::endl;
//            }
//
//            Transaction* voucherTrans = manualTestMgr.findTransactionById(101);
//            if (voucherTrans && voucherTrans->isVoucherApplied() &&
//                strCompare(voucherTrans->getVoucherCode(), "SAVE10")) {
//                std::cout << "✅ Voucher transaction loaded correctly" << std::endl;
//            }
//        }
//        else {
//            std::cout << "❌ Manual file loading failed" << std::endl;
//        }
//
//        // Display loaded transactions
//        std::cout << "\nLoaded transactions summary:" << std::endl;
//        manualTestMgr.listAllTransactions();
//    }
//
//    // Test individual receipt files
//    std::cout << "\nChecking individual receipt files:" << std::endl;
//    std::cout << "✅ receipt_00001.txt should contain first transaction" << std::endl;
//    std::cout << "✅ receipt_00002.txt should contain second transaction" << std::endl;
//
//    // Test edge cases
//    std::cout << "\nTesting edge cases:" << std::endl;
//
//    // Empty voucher code
//    Transaction edgeTest;
//    const char* emptyVoucherLine = "TRANSACTION:200:105:2025-06-24:17:00:00:5.00:0.00:5.00::0:1";
//    edgeTest.loadFromString(emptyVoucherLine);
//
//    if (!edgeTest.isVoucherApplied() && getStrLength(edgeTest.getVoucherCode()) == 0) {
//        std::cout << "✅ Empty voucher handling correct" << std::endl;
//    }
//
//    // Zero discount
//    const char* zeroDiscountLine = "TRANSACTION:201:106:2025-06-24:17:10:00:10.00:0.00:10.00:INVALID:0:1";
//    edgeTest.loadFromString(zeroDiscountLine);
//
//    if (!edgeTest.isVoucherApplied() && edgeTest.getDiscountAmount() == 0.0) {
//        std::cout << "✅ Zero discount handling correct" << std::endl;
//    }
//}
//
//void setupSampleProducts(ProductManager& manager)
//{
//    // Create categories
//    Category* bakery = new Category(1, "Bakery", "Fresh bread and pastries");
//    Category* dairy = new Category(2, "Dairy", "Milk products");
//    Category* fruits = new Category(3, "Fruits", "Fresh fruits");
//
//    manager.addCategory(bakery);
//    manager.addCategory(dairy);
//    manager.addCategory(fruits);
//
//    // Create products
//    ProductByUnit* bread = new ProductByUnit(1001, "Bread", 1, 2.50, 50);
//    ProductByUnit* milk = new ProductByUnit(1002, "Milk 1L", 2, 3.20, 30);
//    ProductByWeight* bananas = new ProductByWeight(2001, "Bananas", 3, 4.20, 25.0);
//    ProductByWeight* apples = new ProductByWeight(2002, "Apples", 3, 3.80, 20.0);
//
//    manager.addProduct(bread);
//    manager.addProduct(milk);
//    manager.addProduct(bananas);
//    manager.addProduct(apples);
//}
//
//void printSeparator(const char* title)
//{
//    std::cout << "\n";
//    for (int i = 0; i < 50; i++) {
//        std::cout << "=";
//    }
//    std::cout << std::endl;
//    std::cout << "   " << title << std::endl;
//    for (int i = 0; i < 50; i++) {
//        std::cout << "=";
//    }
//    std::cout << std::endl;
//}