//#include <iostream>
//#include <fstream>
//#include "voucher.h"
//
////void printSeparator(const char* title) {
////    std::cout << "\n" << "=" * 50 << "\n";
////    std::cout << "  " << title << "\n";
////    std::cout << "=" * 50 << "\n";
////}
//
//void testFactoryCreation() {
//    //printSeparator("TESTING FACTORY CREATION");
//
//    // Test 1: Single Category Gift Card
//    std::cout << "1. Creating Single Category Gift Card (Category 3, 15%):\n";
//    SingleCategoryGiftCard* single = GiftCardFactory::createSingleCategory(3, 15.0);
//    if (single) {
//        single->displayInfo();
//        std::cout << "✅ Single card created successfully!\n";
//    }
//    else {
//        std::cout << "❌ Failed to create single card!\n";
//    }
//
//    // Test 2: Multiple Category Gift Card  
//    std::cout << "\n2. Creating Multiple Category Gift Card (Categories [1,3,5], 20%):\n";
//    int categories[] = { 1, 3, 5 };
//    MultipleCategoryGiftCard* multiple = GiftCardFactory::createMultipleCategory(categories, 3, 20.0);
//    if (multiple) {
//        multiple->displayInfo();
//        std::cout << "✅ Multiple card created successfully!\n";
//    }
//    else {
//        std::cout << "❌ Failed to create multiple card!\n";
//    }
//
//    // Test 3: All Products Gift Card
//    std::cout << "\n3. Creating All Products Gift Card (25%):\n";
//    AllProductsGiftCard* all = GiftCardFactory::createAllProducts(25.0);
//    if (all) {
//        all->displayInfo();
//        std::cout << "✅ All products card created successfully!\n";
//    }
//    else {
//        std::cout << "❌ Failed to create all products card!\n";
//    }
//
//    // Cleanup
//    delete single;
//    delete multiple;
//    delete all;
//}
//
//void testStringParsing() {
//    //printSeparator("TESTING STRING PARSING");
//
//    // Test data
//    const char* test_lines[] = {
//        "SINGLE_CATEGORY:3:15.5",
//        "MULTIPLE_CATEGORY:3:1:2:5:20.0",
//        "ALL_PRODUCTS:25.0"
//    };
//
//    // Test Single Category parsing
//    std::cout << "1. Parsing: " << test_lines[0] << "\n";
//    SingleCategoryGiftCard* single = GiftCardFactory::loadSingleFromString(test_lines[0]);
//    if (single) {
//        std::cout << "✅ Successfully parsed!\n";
//        single->displayInfo();
//        delete single;
//    }
//    else {
//        std::cout << "❌ Failed to parse single category!\n";
//    }
//
//    // Test Multiple Category parsing
//    std::cout << "\n2. Parsing: " << test_lines[1] << "\n";
//    MultipleCategoryGiftCard* multiple = GiftCardFactory::loadMultipleFromString(test_lines[1]);
//    if (multiple) {
//        std::cout << "✅ Successfully parsed!\n";
//        multiple->displayInfo();
//        delete multiple;
//    }
//    else {
//        std::cout << "❌ Failed to parse multiple category!\n";
//    }
//
//    // Test All Products parsing
//    std::cout << "\n3. Parsing: " << test_lines[2] << "\n";
//    AllProductsGiftCard* all = GiftCardFactory::loadAllProductsFromString(test_lines[2]);
//    if (all) {
//        std::cout << "✅ Successfully parsed!\n";
//        all->displayInfo();
//        delete all;
//    }
//    else {
//        std::cout << "❌ Failed to parse all products!\n";
//    }
//}
//
//void testInvalidData() {
//    //printSeparator("TESTING INVALID DATA HANDLING");
//
//    const char* invalid_lines[] = {
//        "SINGLE_CATEGORY:abc:15.5",           // Invalid category
//        "SINGLE_CATEGORY:3:invalid",          // Invalid percentage
//        "MULTIPLE_CATEGORY:0:1:2:3:20.0",     // Zero count
//        "MULTIPLE_CATEGORY:2:1:abc:20.0",     // Invalid category in list
//        "ALL_PRODUCTS:abc",                   // Invalid percentage
//        "UNKNOWN_TYPE:123:456"                // Unknown format
//    };
//
//    for (int i = 0; i < 6; i++) {
//        std::cout << i + 1 << ". Testing invalid: " << invalid_lines[i] << "\n";
//
//        if (invalid_lines[i][0] == 'S') {
//            SingleCategoryGiftCard* card = GiftCardFactory::loadSingleFromString(invalid_lines[i]);
//            if (!card) {
//                std::cout << "✅ Correctly rejected invalid data\n";
//            }
//            else {
//                std::cout << "❌ Should have rejected this data!\n";
//                delete card;
//            }
//        }
//        else if (invalid_lines[i][0] == 'M') {
//            MultipleCategoryGiftCard* card = GiftCardFactory::loadMultipleFromString(invalid_lines[i]);
//            if (!card) {
//                std::cout << "✅ Correctly rejected invalid data\n";
//            }
//            else {
//                std::cout << "❌ Should have rejected this data!\n";
//                delete card;
//            }
//        }
//        else if (invalid_lines[i][0] == 'A') {
//            AllProductsGiftCard* card = GiftCardFactory::loadAllProductsFromString(invalid_lines[i]);
//            if (!card) {
//                std::cout << "✅ Correctly rejected invalid data\n";
//            }
//            else {
//                std::cout << "❌ Should have rejected this data!\n";
//                delete card;
//            }
//        }
//        else {
//            std::cout << "✅ Unknown format correctly ignored\n";
//        }
//    }
//}
//
//void testFileOperations() {
//    //printSeparator("TESTING FILE OPERATIONS");
//
//    // Create test file
//    std::cout << "1. Creating test file with voucher data...\n";
//    std::ofstream outFile("test_vouchers.txt");
//    outFile << "SINGLE_CATEGORY:3:15.5\n";
//    outFile << "MULTIPLE_CATEGORY:2:1:4:20.0\n";
//    outFile << "ALL_PRODUCTS:10.0\n";
//    outFile << "SINGLE_CATEGORY:5:25.0\n";
//    outFile.close();
//    std::cout << "✅ Test file created!\n";
//
//    // Test loading from file using wrapper functions
//    std::cout << "\n2. Testing loadFromFile() wrapper functions...\n";
//    std::ifstream inFile("test_vouchers.txt");
//
//    // Load first card (Single)
//    SingleCategoryGiftCard card1;
//    if (card1.loadFromFile(inFile)) {
//        std::cout << "✅ Single card loaded from file:\n";
//        card1.displayInfo();
//    }
//    else {
//        std::cout << "❌ Failed to load single card\n";
//    }
//
//    // Load second card (Multiple)
//    MultipleCategoryGiftCard card2;
//    if (card2.loadFromFile(inFile)) {
//        std::cout << "\n✅ Multiple card loaded from file:\n";
//        card2.displayInfo();
//    }
//    else {
//        std::cout << "❌ Failed to load multiple card\n";
//    }
//
//    // Load third card (All Products)
//    AllProductsGiftCard card3;
//    if (card3.loadFromFile(inFile)) {
//        std::cout << "\n✅ All products card loaded from file:\n";
//        card3.displayInfo();
//    }
//    else {
//        std::cout << "❌ Failed to load all products card\n";
//    }
//
//    inFile.close();
//}
//
//void testVoucherLogic() {
//    //printSeparator("TESTING VOUCHER APPLICATION LOGIC");
//
//    // Create test vouchers
//    SingleCategoryGiftCard* single = GiftCardFactory::createSingleCategory(3, 15.0);
//    int cats[] = { 1, 3, 5 };
//    MultipleCategoryGiftCard* multiple = GiftCardFactory::createMultipleCategory(cats, 3, 20.0);
//    AllProductsGiftCard* all = GiftCardFactory::createAllProducts(10.0);
//
//    double test_price = 100.0;
//
//    std::cout << "Testing with product price: $" << test_price << "\n\n";
//
//    // Test different categories
//    int test_categories[] = { 1, 3, 7 };
//    const char* cat_names[] = { "Foods", "Fruits", "Electronics" };
//
//    for (int i = 0; i < 3; i++) {
//        int cat_id = test_categories[i];
//        std::cout << "Category " << cat_id << " (" << cat_names[i] << "):\n";
//
//        // Test Single Category voucher
//        if (single->canApplyToCategory(cat_id)) {
//            double discount = single->calculateDiscount(test_price, cat_id);
//            std::cout << "  Single voucher: $" << discount << " discount ✅\n";
//        }
//        else {
//            std::cout << "  Single voucher: Not applicable ❌\n";
//        }
//
//        // Test Multiple Category voucher
//        if (multiple->canApplyToCategory(cat_id)) {
//            double discount = multiple->calculateDiscount(test_price, cat_id);
//            std::cout << "  Multiple voucher: $" << discount << " discount ✅\n";
//        }
//        else {
//            std::cout << "  Multiple voucher: Not applicable ❌\n";
//        }
//
//        // Test All Products voucher
//        if (all->canApplyToCategory(cat_id)) {
//            double discount = all->calculateDiscount(test_price, cat_id);
//            std::cout << "  All products voucher: $" << discount << " discount ✅\n";
//        }
//        else {
//            std::cout << "  All products voucher: Not applicable ❌\n";
//        }
//
//        std::cout << "\n";
//    }
//
//    // Test used voucher
//    std::cout << "Testing used voucher behavior:\n";
//    single->markAsUsed();
//    if (!single->canApplyToCategory(3)) {
//        std::cout << "✅ Used voucher correctly rejected\n";
//    }
//    else {
//        std::cout << "❌ Used voucher should not be applicable!\n";
//    }
//
//    // Cleanup
//    delete single;
//    delete multiple;
//    delete all;
//}
//
//void testMemoryManagement() {
////printSeparator("TESTING MEMORY MANAGEMENT");
//
//    std::cout << "1. Testing Rule of Six operations...\n";
//
//    // Create original objects
//    int cats[] = { 1, 2, 3 };
//    MultipleCategoryGiftCard* original = GiftCardFactory::createMultipleCategory(cats, 3, 15.0);
//
//    std::cout << "Original card:\n";
//    original->displayInfo();
//
//    // Test copy constructor
//    MultipleCategoryGiftCard copy_constructed(*original);
//    std::cout << "\nCopy constructed card:\n";
//    copy_constructed.displayInfo();
//
//    // Test assignment operator
//    MultipleCategoryGiftCard assigned;
//    assigned = *original;
//    std::cout << "\nAssigned card:\n";
//    assigned.displayInfo();
//
//    // Test that they're independent (modify original)
//    original->addCategory(7);
//    std::cout << "\nAfter adding category 7 to original:\n";
//    std::cout << "Original categories: " << original->getCategoryCount() << "\n";
//    std::cout << "Copy categories: " << copy_constructed.getCategoryCount() << "\n";
//    std::cout << "Assigned categories: " << assigned.getCategoryCount() << "\n";
//
//    if (copy_constructed.getCategoryCount() != original->getCategoryCount()) {
//        std::cout << "✅ Deep copy working correctly!\n";
//    }
//    else {
//        std::cout << "❌ Deep copy failed - shallow copy detected!\n";
//    }
//
//    delete original;
//    std::cout << "✅ Memory management test completed\n";
//}
//
//void testSaveToFile() {
//    //printSeparator("TESTING SAVE TO FILE");
//
//    std::cout << "Creating vouchers and saving to file...\n";
//
//    // Create various vouchers
//    SingleCategoryGiftCard* single = GiftCardFactory::createSingleCategory(5, 30.0);
//    int cats[] = { 2, 4, 6 };
//    MultipleCategoryGiftCard* multiple = GiftCardFactory::createMultipleCategory(cats, 3, 25.0);
//    AllProductsGiftCard* all = GiftCardFactory::createAllProducts(12.5);
//
//    // Mark one as used
//    multiple->markAsUsed();
//
//    // Save to file
//    std::ofstream saveFile("saved_vouchers.txt");
//    single->saveToFile(saveFile);
//    multiple->saveToFile(saveFile);
//    all->saveToFile(saveFile);
//    saveFile.close();
//
//    std::cout << "✅ Vouchers saved to saved_vouchers.txt\n";
//
//    // Verify by reading and displaying
//    std::cout << "\nVerifying saved data:\n";
//    std::ifstream verifyFile("saved_vouchers.txt");
//    char line[512];
//    int line_num = 1;
//
//    while (verifyFile.getline(line, 512)) {
//        std::cout << "Line " << line_num << ": " << line << "\n";
//        line_num++;
//    }
//    verifyFile.close();
//
//    // Cleanup
//    delete single;
//    delete multiple;
//    delete all;
//}
//
//int main() {
//    std::cout << "🎟️ VOUCHER SYSTEM COMPREHENSIVE TESTING 🎟️\n";
//    std::cout << "Testing all functionality of the Gift Card Template System\n";
//
//    try {
//        testFactoryCreation();
//        testStringParsing();
//        testInvalidData();
//        testFileOperations();
//        testVoucherLogic();
//        testMemoryManagement();
//        testSaveToFile();
//
//        //printSeparator("ALL TESTS COMPLETED");
//        std::cout << "🎉 Voucher system testing finished!\n";
//        std::cout << "Check the generated files:\n";
//        std::cout << "  - test_vouchers.txt (input test data)\n";
//        std::cout << "  - saved_vouchers.txt (output test data)\n";
//
//    }
//    catch (const std::exception& e) {
//        std::cout << "❌ Exception occurred: " << e.what() << "\n";
//    }
//    catch (...) {
//        std::cout << "❌ Unknown exception occurred!\n";
//    }
//
//    return 0;
//}