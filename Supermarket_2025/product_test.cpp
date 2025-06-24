//#include "product.h"
//#include <iostream>
//
//void testCategorySystem(ProductManager& manager);
//void testProductSystem(ProductManager& manager);
//void testStockManagement(ProductManager& manager);
//void testFileOperations(ProductManager& manager);
//void printSeparator(const char* title);
//
//int main()
//{
//    std::cout << "=== PRODUCT SYSTEM TEST ===" << std::endl;
//
//    ProductManager manager;
//
//    testCategorySystem(manager);
//    testProductSystem(manager);
//    testStockManagement(manager);
//    testFileOperations(manager);
//
//    std::cout << "\n=== TEST COMPLETED ===" << std::endl;
//    return 0;
//}
//
//void testCategorySystem(ProductManager& manager)
//{
//    printSeparator("CATEGORY SYSTEM TEST");
//
//    // Create categories
//    Category* fruits = new Category(Category::generateNewCategoryId(), "Fruits", "Fresh fruits and vegetables");
//    Category* dairy = new Category(Category::generateNewCategoryId(), "Dairy", "Milk products and cheese");
//    Category* bakery = new Category(Category::generateNewCategoryId(), "Bakery", "Fresh bread and pastries");
//
//    // Add categories
//    std::cout << "Adding categories..." << std::endl;
//    if (manager.addCategory(fruits)) {
//        std::cout << "✅ Fruits category added" << std::endl;
//    }
//    if (manager.addCategory(dairy)) {
//        std::cout << "✅ Dairy category added" << std::endl;
//    }
//    if (manager.addCategory(bakery)) {
//        std::cout << "✅ Bakery category added" << std::endl;
//    }
//
//    // Try to add duplicate
//    Category* duplicateFruits = new Category(Category::generateNewCategoryId(), "Fruits", "Duplicate test");
//    if (!manager.addCategory(duplicateFruits)) {
//        std::cout << "✅ Duplicate category rejected correctly" << std::endl;
//        delete duplicateFruits;
//    }
//
//    // List all categories
//    std::cout << "\nListing all categories:" << std::endl;
//    manager.listAllCategories();
//
//    // Find category tests
//    std::cout << "\nTesting category search:" << std::endl;
//    Category* found = manager.findCategoryByName("Dairy");
//    if (found) {
//        std::cout << "✅ Found category by name: " << found->getName() << " (ID: " << found->getId() << ")" << std::endl;
//    }
//
//    found = manager.findCategoryById(1);
//    if (found) {
//        std::cout << "✅ Found category by ID: " << found->getName() << std::endl;
//    }
//}
//
//void testProductSystem(ProductManager& manager)
//{
//    printSeparator("PRODUCT SYSTEM TEST");
//
//    // Create products by unit
//    ProductByUnit* bread = new ProductByUnit(
//        Product::generateNewProductId(),
//        "Bread",
//        3, // Bakery category
//        2.50,
//        50
//    );
//
//    ProductByUnit* milk = new ProductByUnit(
//        Product::generateNewProductId(),
//        "Milk 1L",
//        2, // Dairy category
//        3.20,
//        100
//    );
//
//    // Create products by weight
//    ProductByWeight* bananas = new ProductByWeight(
//        Product::generateNewProductId(),
//        "Bananas",
//        1, // Fruits category
//        4.20,
//        25.5
//    );
//
//    ProductByWeight* cheese = new ProductByWeight(
//        Product::generateNewProductId(),
//        "Bulgarian Cheese",
//        2, // Dairy category
//        12.80,
//        8.5
//    );
//
//    // Add products
//    std::cout << "Adding products..." << std::endl;
//    if (manager.addProduct(bread)) {
//        std::cout << "✅ Bread added (ID: " << bread->getId() << ")" << std::endl;
//    }
//    if (manager.addProduct(milk)) {
//        std::cout << "✅ Milk added (ID: " << milk->getId() << ")" << std::endl;
//    }
//    if (manager.addProduct(bananas)) {
//        std::cout << "✅ Bananas added (ID: " << bananas->getId() << ")" << std::endl;
//    }
//    if (manager.addProduct(cheese)) {
//        std::cout << "✅ Cheese added (ID: " << cheese->getId() << ")" << std::endl;
//    }
//
//    // List all products
//    std::cout << "\nListing all products:" << std::endl;
//    manager.listAllProducts();
//
//    // List products by category
//    std::cout << "\nProducts in Dairy category (ID: 2):" << std::endl;
//    manager.listProductsByCategory(2);
//
//    // Find product tests
//    std::cout << "\nTesting product search:" << std::endl;
//    Product* foundProduct = manager.findProductByName("Bananas");
//    if (foundProduct) {
//        std::cout << "✅ Found product: " << foundProduct->getName()
//            << " (Type: " << foundProduct->getType() << ")" << std::endl;
//    }
//
//    foundProduct = manager.findProductById(bread->getId());
//    if (foundProduct) {
//        std::cout << "✅ Found product by ID: " << foundProduct->getName() << std::endl;
//    }
//}
//
//void testStockManagement(ProductManager& manager)
//{
//    printSeparator("STOCK MANAGEMENT TEST");
//
//    // Test stock checking
//    std::cout << "Testing stock availability:" << std::endl;
//
//    Product* bread = manager.findProductByName("Bread");
//    if (bread) {
//        std::cout << "Bread available quantity: " << bread->getAvailableQuantity() << " units" << std::endl;
//
//        if (bread->hasEnoughQuantity(5)) {
//            std::cout << "✅ Has enough bread for 5 units" << std::endl;
//        }
//
//        if (!bread->hasEnoughQuantity(100)) {
//            std::cout << "✅ Correctly reports insufficient stock for 100 units" << std::endl;
//        }
//    }
//
//    Product* bananas = manager.findProductByName("Bananas");
//    if (bananas) {
//        std::cout << "Bananas available quantity: " << bananas->getAvailableQuantity() << " kg" << std::endl;
//
//        if (bananas->hasEnoughQuantity(2.5)) {
//            std::cout << "✅ Has enough bananas for 2.5 kg" << std::endl;
//        }
//    }
//
//    // Test selling (stock reduction)
//    std::cout << "\nTesting stock reduction (simulating sales):" << std::endl;
//
//    if (bread) {
//        std::cout << "Before sale - Bread quantity: " << bread->getAvailableQuantity() << std::endl;
//
//        if (manager.updateProductStock(bread->getId(), 3, false)) { // Sell 3 units
//            std::cout << "✅ Sold 3 units of bread" << std::endl;
//            std::cout << "After sale - Bread quantity: " << bread->getAvailableQuantity() << std::endl;
//        }
//    }
//
//    if (bananas) {
//        std::cout << "Before sale - Bananas quantity: " << bananas->getAvailableQuantity() << std::endl;
//
//        if (manager.updateProductStock(bananas->getId(), 1.5, false)) { // Sell 1.5 kg
//            std::cout << "✅ Sold 1.5 kg of bananas" << std::endl;
//            std::cout << "After sale - Bananas quantity: " << bananas->getAvailableQuantity() << std::endl;
//        }
//    }
//
//    // Test restocking (stock addition)
//    std::cout << "\nTesting stock addition (restocking):" << std::endl;
//
//    if (bread) {
//        if (manager.updateProductStock(bread->getId(), 20, true)) { // Add 20 units
//            std::cout << "✅ Restocked 20 units of bread" << std::endl;
//            std::cout << "After restock - Bread quantity: " << bread->getAvailableQuantity() << std::endl;
//        }
//    }
//}
//
//void testFileOperations(ProductManager& manager)
//{
//    printSeparator("FILE I/O OPERATIONS TEST");
//
//    // Save categories and products
//    std::cout << "Saving data to files..." << std::endl;
//    manager.saveAllCategories("test_categories.txt");
//    manager.saveAllProducts("test_products.txt");
//    std::cout << "✅ Data saved to test_categories.txt and test_products.txt" << std::endl;
//
//    // Create new manager and load data
//    std::cout << "\nCreating new ProductManager and loading data..." << std::endl;
//    ProductManager newManager;
//
//    newManager.loadAllCategories("test_categories.txt");
//    newManager.loadAllProducts("test_products.txt");
//
//    std::cout << "✅ Data loaded successfully" << std::endl;
//    std::cout << "Categories loaded: " << newManager.getCategoryCount() << std::endl;
//    std::cout << "Products loaded: " << newManager.getProductCount() << std::endl;
//
//    // Verify loaded data
//    std::cout << "\nVerifying loaded data:" << std::endl;
//    std::cout << "Loaded categories:" << std::endl;
//    newManager.listAllCategories();
//
//    std::cout << "\nLoaded products:" << std::endl;
//    newManager.listAllProducts();
//
//    // Test stock receipt loading
//    std::cout << "\nTesting stock receipt loading..." << std::endl;
//
//    // Create a test stock receipt file
//    std::ofstream receiptFile("test_stock_receipt.txt");
//    if (receiptFile.is_open()) {
//        receiptFile << "PRODUCT_UNIT>>1001>>10" << std::endl;  // Add 10 units to bread
//        receiptFile << "PRODUCT_WEIGHT>>1003>>5.0" << std::endl; // Add 5.0 kg to bananas
//        receiptFile.close();
//
//        std::cout << "Before loading receipt:" << std::endl;
//        Product* testBread = newManager.findProductById(1001);
//        Product* testBananas = newManager.findProductById(1003);
//
//        if (testBread) {
//            std::cout << "Bread quantity: " << testBread->getAvailableQuantity() << std::endl;
//        }
//        if (testBananas) {
//            std::cout << "Bananas quantity: " << testBananas->getAvailableQuantity() << std::endl;
//        }
//
//        newManager.loadProductsFromReceipt("test_stock_receipt.txt");
//
//        std::cout << "After loading receipt:" << std::endl;
//        if (testBread) {
//            std::cout << "Bread quantity: " << testBread->getAvailableQuantity() << std::endl;
//        }
//        if (testBananas) {
//            std::cout << "Bananas quantity: " << testBananas->getAvailableQuantity() << std::endl;
//        }
//
//        std::cout << "✅ Stock receipt loaded successfully" << std::endl;
//    }
//
//    // Test category deletion with products protection
//    std::cout << "\nTesting category deletion protection:" << std::endl;
//
//    if (!newManager.removeCategoryById(2)) { // Dairy category has products
//        std::cout << "✅ Category with products cannot be deleted (correct behavior)" << std::endl;
//    }
//
//    // Remove products first, then category
//    newManager.removeProductById(1002); // Remove milk
//    newManager.removeProductById(1004); // Remove cheese
//
//    if (newManager.removeCategoryById(2)) { // Now dairy category is empty
//        std::cout << "✅ Empty category deleted successfully" << std::endl;
//    }
//}
//
//void printSeparator(const char* title)
//{
//    std::cout << "\n" << std::string(50, '=') << std::endl;
//    std::cout << "   " << title << std::endl;
//    std::cout << std::string(50, '=') << std::endl;
//}