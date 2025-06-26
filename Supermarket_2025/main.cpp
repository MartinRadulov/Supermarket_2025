#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "utils.h"
#include "auth.h"
#include "worker_manager.h"
#include "product.h"
#include "transaction.h"
#include "voucher.h"
#include "workers.h"

class CLISystem {
private:
    AuthSystem auth;
    WorkerManager workerMgr;
    ProductManager productMgr;
    TransactionManager transactionMgr;

    SingleCategoryGiftCard** singleVouchers;
    MultipleCategoryGiftCard** multipleVouchers;
    AllProductsGiftCard** allVouchers;
    int singleVoucherCount;
    int multipleVoucherCount;
    int allVoucherCount;
    int voucherCapacity;

    bool systemRunning;
    char inputBuffer[1000];
    char commandBuffer[100];
    char argsBuffer[900];

public:
    CLISystem() : systemRunning(true), singleVoucherCount(0), multipleVoucherCount(0),
        allVoucherCount(0), voucherCapacity(10)
    {
        srand(static_cast<unsigned int>(time(nullptr)));

        singleVouchers = new SingleCategoryGiftCard * [voucherCapacity];
        multipleVouchers = new MultipleCategoryGiftCard * [voucherCapacity];
        allVouchers = new AllProductsGiftCard * [voucherCapacity];

        auth.setWorkerManager(&workerMgr);
        transactionMgr.setProductManager(&productMgr);

        loadSystemData();
        createDefaultManagerIfNeeded();
    }

    ~CLISystem()
    {
        saveSystemData();

        for (int i = 0; i < singleVoucherCount; i++)
        {
            delete singleVouchers[i];
        }
        for (int i = 0; i < multipleVoucherCount; i++)
        {
            delete multipleVouchers[i];
        }
        for (int i = 0; i < allVoucherCount; i++)
        {
            delete allVouchers[i];
        }

        delete[] singleVouchers;
        delete[] multipleVouchers;
        delete[] allVouchers;
    }

    void run()
    {
        displayWelcomeMessage();

        while (systemRunning)
        {
            displayPrompt();
            readInput();

            if (parseCommand())
            {
                executeCommand();
            }
        }

        displayGoodbyeMessage();
    }

private:
    void loadSystemData()
    {
        std::cout << "Loading system data..." << std::endl;

        std::ifstream categoryFile("categories.txt");
        if (categoryFile.is_open())
        {
            productMgr.loadAllCategories("categories.txt");
            categoryFile.close();
        }

        std::ifstream productFile("products.txt");
        if (productFile.is_open())
        {
            productMgr.loadAllProducts("products.txt");
            productFile.close();
        }

        std::ifstream transactionFile("transactions.txt");
        if (transactionFile.is_open())
        {
            transactionMgr.loadAllTransactions("transactions.txt");
            transactionFile.close();
        }

        std::ifstream voucherFile("vouchers.txt");
        if (voucherFile.is_open())
        {
            voucherFile.close();
            loadAllVouchers("vouchers.txt");
        }

        std::cout << "System data loaded successfully." << std::endl;
    }



    void saveAllVouchers(const char* filename) const
    {
        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cout << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        for (int i = 0; i < singleVoucherCount; i++)
        {
            if (singleVouchers[i])
            {
                singleVouchers[i]->saveToFile(file);
            }
        }

        for (int i = 0; i < multipleVoucherCount; i++)
        {
            if (multipleVouchers[i])
            {
                multipleVouchers[i]->saveToFile(file);
            }
        }

        for (int i = 0; i < allVoucherCount; i++)
        {
            if (allVouchers[i])
            {
                allVouchers[i]->saveToFile(file);
            }
        }

        file.close();
        std::cout << "Vouchers saved successfully to " << filename << std::endl;
    }

    void loadAllVouchers(const char* filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout << "Voucher file not found: " << filename << std::endl;
            return;
        }

        char line[512];
        int loadedCount = 0;

        while (file.getline(line, sizeof(line)))
        {
            trimStr(line);
            if (getStrLength(line) == 0) continue;

            if (line[0] == 'S' && line[1] == 'I') // SINGLE_CATEGORY
            {
                SingleCategoryGiftCard* voucher = GiftCardFactory::loadSingleFromString(line);
                if (voucher && singleVoucherCount < voucherCapacity)
                {
                    singleVouchers[singleVoucherCount] = voucher;
                    singleVoucherCount++;
                    loadedCount++;
                }
            }
            else if (line[0] == 'M' && line[1] == 'U') // MULTIPLE_CATEGORY
            {
                MultipleCategoryGiftCard* voucher = GiftCardFactory::loadMultipleFromString(line);
                if (voucher && multipleVoucherCount < voucherCapacity)
                {
                    multipleVouchers[multipleVoucherCount] = voucher;
                    multipleVoucherCount++;
                    loadedCount++;
                }
            }
            else if (line[0] == 'A' && line[1] == 'L') // ALL_PRODUCTS
            {
                AllProductsGiftCard* voucher = GiftCardFactory::loadAllProductsFromString(line);
                if (voucher && allVoucherCount < voucherCapacity)
                {
                    allVouchers[allVoucherCount] = voucher;
                    allVoucherCount++;
                    loadedCount++;
                }
            }
        }

        file.close();
        std::cout << "Loaded " << loadedCount << " vouchers successfully from " << filename << std::endl;
    }

    void clearAllVouchers()
    {
        for (int i = 0; i < singleVoucherCount; i++)
        {
            delete singleVouchers[i];
            singleVouchers[i] = nullptr;
        }
        singleVoucherCount = 0;

        for (int i = 0; i < multipleVoucherCount; i++)
        {
            delete multipleVouchers[i];
            multipleVouchers[i] = nullptr;
        }
        multipleVoucherCount = 0;

        for (int i = 0; i < allVoucherCount; i++)
        {
            delete allVouchers[i];
            allVouchers[i] = nullptr;
        }
        allVoucherCount = 0;
    }

    void saveSystemData()
    {
        std::cout << "Saving system data..." << std::endl;

        productMgr.saveAllCategories("categories.txt");
        productMgr.saveAllProducts("products.txt");
        transactionMgr.saveAllTransactions("transactions.txt");

        saveAllVouchers("vouchers.txt");

        std::cout << "System data saved successfully." << std::endl;
    }

    void createDefaultManagerIfNeeded()
    {
        if (workerMgr.getWorkerCount() == 0)
        {
            std::cout << "Creating default manager account..." << std::endl;

            Manager* defaultMgr = new Manager(Worker::generateNewId(), "Admin", "Manager",
                "0881234567", 30, "admin123");
            defaultMgr->setApproved(true);

            if (workerMgr.addWorker(defaultMgr))
            {
                defaultMgr->generateSpecialCode();
                std::cout << "Default manager created with ID: " << defaultMgr->getId()
                    << ", Password: admin123" << std::endl;
                std::cout << "Special code: " << defaultMgr->getSpecialCode() << std::endl;
            }
            else
            {
                std::cout << "Failed to add default manager!" << std::endl;
                delete defaultMgr;
            }
        }
    }

    void displayWelcomeMessage()
    {
        std::cout << std::endl;
        std::cout << "=================================================" << std::endl;
        std::cout << "  Welcome to FMI Supermarket Management System  " << std::endl;
        std::cout << "                   2024-2025                    " << std::endl;
        std::cout << "=================================================" << std::endl;
        std::cout << std::endl;

        if (!auth.isLoggedIn())
        {
            std::cout << "Please login to continue." << std::endl;
            std::cout << "Available commands:" << std::endl;
            std::cout << "  login <id> <password>" << std::endl;
            std::cout << "  register <role> <first_name> <last_name> <phone> <age> <password>" << std::endl;
            std::cout << "  exit" << std::endl;
        }
    }

    void displayPrompt()
    {
        if (auth.isLoggedIn())
        {
            std::cout << "[" << auth.getCurrentUserRole() << " "
                << auth.getCurrentUserId() << "] > ";
        }
        else
        {
            std::cout << "> ";
        }
    }

    void displayGoodbyeMessage()
    {
        std::cout << std::endl;
        std::cout << "Thank you for using FMI Supermarket Management System!" << std::endl;
        std::cout << "Goodbye!" << std::endl;
    }

    void readInput()
    {
        std::cin.getline(inputBuffer, sizeof(inputBuffer));
        trimStr(inputBuffer);
    }

    bool parseCommand()
    {
        if (getStrLength(inputBuffer) == 0)
        {
            return false;
        }

        int spacePos = -1;
        for (int i = 0; inputBuffer[i] != '\0'; i++)
        {
            if (isWhitespace(inputBuffer[i]))
            {
                spacePos = i;
                break;
            }
        }

        if (spacePos == -1)
        {
            strCopy(commandBuffer, inputBuffer, sizeof(commandBuffer));
            argsBuffer[0] = '\0';
        }
        else
        {
            int cmdLen = (spacePos < sizeof(commandBuffer) - 1) ? spacePos : sizeof(commandBuffer) - 1;
            for (int i = 0; i < cmdLen; i++)
            {
                commandBuffer[i] = inputBuffer[i];
            }
            commandBuffer[cmdLen] = '\0';

            int argStart = spacePos;
            while (inputBuffer[argStart] != '\0' && isWhitespace(inputBuffer[argStart]))
            {
                argStart++;
            }
            strCopy(argsBuffer, &inputBuffer[argStart], sizeof(argsBuffer));
        }

        return true;
    }

    void executeCommand()
    {
        for (int i = 0; commandBuffer[i] != '\0'; i++)
        {
            commandBuffer[i] = toLowerCase(commandBuffer[i]);
        }

        if (strCompare(commandBuffer, "exit") || strCompare(commandBuffer, "quit"))
        {
            systemRunning = false;
            return;
        }

        if (strCompare(commandBuffer, "login"))
        {
            executeLogin();
            return;
        }

        if (strCompare(commandBuffer, "register"))
        {
            executeRegister();
            return;
        }

        if (!auth.isLoggedIn())
        {
            auth.displayNotLoggedInMessage();
            return;
        }

        if (!auth.canExecuteCommand(commandBuffer))
        {
            auth.displayAccessDeniedMessage(commandBuffer);
            return;
        }

        if (strCompare(commandBuffer, "logout"))
        {
            auth.logout();
        }
        else if (strCompare(commandBuffer, "list-user-data"))
        {
            auth.displayCurrentUserInfo();
        }
        else if (strCompare(commandBuffer, "list-workers"))
        {
            workerMgr.listAllWorkers();
        }
        else if (strCompare(commandBuffer, "list-products"))
        {
            executeListProducts();
        }
        else if (strCompare(commandBuffer, "list-transactions"))
        {
            transactionMgr.listAllTransactions();
        }
        else if (strCompare(commandBuffer, "sell"))
        {
            executeSell();
        }
        else if (strCompare(commandBuffer, "list-pending"))
        {
            workerMgr.listPendingWorkers();
        }
        else if (strCompare(commandBuffer, "approve"))
        {
            executeApprove();
        }
        else if (strCompare(commandBuffer, "decline"))
        {
            executeDecline();
        }
        else if (strCompare(commandBuffer, "warn-cashier"))
        {
            executeWarnCashier();
        }
        else if (strCompare(commandBuffer, "promote-cashier"))
        {
            executePromoteCashier();
        }
        else if (strCompare(commandBuffer, "fire-cashier"))
        {
            executeFireCashier();
        }
        else if (strCompare(commandBuffer, "add-category"))
        {
            executeAddCategory();
        }
        else if (strCompare(commandBuffer, "delete-category"))
        {
            executeDeleteCategory();
        }
        else if (strCompare(commandBuffer, "add-product"))
        {
            executeAddProduct();
        }
        else if (strCompare(commandBuffer, "delete-product"))
        {
            executeDeleteProduct();
        }
        else if (strCompare(commandBuffer, "load-products"))
        {
            executeLoadProducts();
        }
        else if (strCompare(commandBuffer, "load-gift-cards"))
        {
            executeLoadGiftCards();
        }
        else if (strCompare(commandBuffer, "create-single-voucher"))
        {
            executeCreateSingleVoucher();
        }
        else if (strCompare(commandBuffer, "create-multiple-voucher"))
        {
            executeCreateMultipleVoucher();
        }
        else if (strCompare(commandBuffer, "create-all-voucher"))
        {
            executeCreateAllVoucher();
        }
        else if (strCompare(commandBuffer, "list-vouchers"))
        {
            executeListVouchers();
        }
        else if (strCompare(commandBuffer, "save-vouchers"))
        {
            executeManualSaveVouchers();
        }
        else
        {
            std::cout << "Unknown command: " << commandBuffer << std::endl;
            std::cout << "Available commands for " << auth.getCurrentUserRole() << ":" << std::endl;

            if (auth.isManager()) {
                std::cout << "Manager commands: approve, decline, warn-cashier, promote-cashier, fire-cashier" << std::endl;
                std::cout << "Product commands: add-category, delete-category, add-product, delete-product, load-products" << std::endl;
                std::cout << "Voucher commands: create-single-voucher, create-multiple-voucher, create-all-voucher, load-gift-cards" << std::endl;
            }
            if (auth.isCashier()) {
                std::cout << "Cashier commands: sell" << std::endl;
            }
            std::cout << "Common commands: list-user-data, list-workers, list-products, list-transactions, list-vouchers, logout" << std::endl;
        }
    }

    void executeManualSaveVouchers()
    {
        if (!auth.isManager())
        {
            std::cout << "Only managers can save vouchers." << std::endl;
            return;
        }

        if (getStrLength(argsBuffer) == 0)
        {
            saveAllVouchers("vouchers.txt");
        }
        else
        {
            saveAllVouchers(argsBuffer);
        }
    }

    void executeLogin()
    {
        char idStr[20], password[100];
        if (!parseLoginArgs(argsBuffer, idStr, password))
        {
            std::cout << "Usage: login <worker_id> <password>" << std::endl;
            return;
        }

        if (!isValidNumber(idStr))
        {
            std::cout << "Invalid worker ID format." << std::endl;
            return;
        }

        int workerId = 0;
        for (int i = 0; idStr[i] != '\0'; i++)
        {
            workerId = workerId * 10 + (idStr[i] - '0');
        }

        auth.login(workerId, password);
    }

    void executeRegister()
    {
        char role[20], firstName[50], lastName[50], phone[20], ageStr[10], password[100];

        if (!parseRegisterArgs(argsBuffer, role, firstName, lastName, phone, ageStr, password))
        {
            std::cout << "Usage: register <role> <first_name> <last_name> <phone> <age> <password>" << std::endl;
            std::cout << "Roles: cashier, manager" << std::endl;
            return;
        }

        if (!isValidNumber(ageStr))
        {
            std::cout << "Invalid age format." << std::endl;
            return;
        }

        int age = 0;
        for (int i = 0; ageStr[i] != '\0'; i++)
        {
            age = age * 10 + (ageStr[i] - '0');
        }

        if (age < 18 || age > 100)
        {
            std::cout << "Age must be between 18 and 100." << std::endl;
            return;
        }

        Worker* newWorker = nullptr;
        int newId = Worker::generateNewId();

        if (strCompare(role, "cashier"))
        {
            newWorker = new Cashier(newId, firstName, lastName, phone, age, password);
        }
        else if (strCompare(role, "manager"))
        {
            Manager* newMgr = new Manager(newId, firstName, lastName, phone, age, password);
            newMgr->generateSpecialCode();
            newMgr->setApproved(true);
            newWorker = newMgr;
        }
        else
        {
            std::cout << "Invalid role. Use 'cashier' or 'manager'." << std::endl;
            return;
        }

        if (workerMgr.addWorker(newWorker))
        {
            if (strCompare(role, "cashier"))
            {
                std::cout << "Cashier registered successfully with ID: " << newWorker->getId() << std::endl;
                std::cout << "Registration pending approval from a manager." << std::endl;
                std::cout << "You can login after manager approval using: login " << newWorker->getId() << " " << password << std::endl;
            }
            else
            {
                std::cout << "Manager registered successfully with ID: " << newWorker->getId() << std::endl;
                Manager* mgr = static_cast<Manager*>(newWorker);
                std::cout << "Special code: " << mgr->getSpecialCode() << std::endl;
                std::cout << "You can login immediately using: login " << newWorker->getId() << " " << password << std::endl;
                char filename[50];
                intToString(newWorker->getId(), filename, 20);
                int len = getStrLength(filename);
                const char* suffix = "_special_code.txt";
                for (int i = 0; suffix[i] != '\0' && len + i < 49; i++)
                {
                    filename[len + i] = suffix[i];
                }
                filename[len + getStrLength(suffix)] = '\0';
                std::cout << "Code saved to: " << filename << std::endl;
            }
        }
        else
        {
            std::cout << "Failed to register worker." << std::endl;
            delete newWorker;
        }
    }

    bool parseRegisterArgs(const char* args, char* role, char* firstName, char* lastName,
        char* phone, char* ageStr, char* password)
    {
        char temp[900];
        strCopy(temp, args, sizeof(temp));

        char* parts[6];
        int partCount = 0;
        int start = 0;
        int len = getStrLength(temp);

        for (int i = 0; i <= len && partCount < 6; i++) {
            if (temp[i] == ' ' || temp[i] == '\0') {
                if (i > start) {
                    temp[i] = '\0';
                    parts[partCount] = &temp[start];
                    partCount++;
                }

                while (i + 1 <= len && temp[i + 1] == ' ') {
                    i++;
                }
                start = i + 1;
            }
        }

        if (partCount != 6) {
            return false;
        }

        strCopy(role, parts[0], 20);
        strCopy(firstName, parts[1], 50);
        strCopy(lastName, parts[2], 50);
        strCopy(phone, parts[3], 20);
        strCopy(ageStr, parts[4], 10);
        strCopy(password, parts[5], 100);

        return true;
    }

    void executeListProducts()
    {
        if (getStrLength(argsBuffer) == 0)
        {
            productMgr.listAllProducts();
        }
        else
        {
            if (!isValidNumber(argsBuffer))
            {
                std::cout << "Invalid category ID format." << std::endl;
                return;
            }

            int categoryId = 0;
            for (int i = 0; argsBuffer[i] != '\0'; i++)
            {
                categoryId = categoryId * 10 + (argsBuffer[i] - '0');
            }

            productMgr.listProductsByCategory(categoryId);
        }
    }

    void executeAddCategory()
    {
        char name[100], description[300];
        if (!parseCategoryArgs(argsBuffer, name, description))
        {
            std::cout << "Usage: add-category <n> <description>" << std::endl;
            return;
        }

        Category* newCategory = new Category(Category::generateNewCategoryId(), name, description);
        if (productMgr.addCategory(newCategory))
        {
            std::cout << "Category \"" << name << "\" added successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to add category. Category may already exist." << std::endl;
            delete newCategory;
        }
    }

    void executeDeleteCategory()
    {
        if (!isValidNumber(argsBuffer))
        {
            std::cout << "Usage: delete-category <category_id>" << std::endl;
            return;
        }

        int categoryId = 0;
        for (int i = 0; argsBuffer[i] != '\0'; i++)
        {
            categoryId = categoryId * 10 + (argsBuffer[i] - '0');
        }

        if (productMgr.removeCategoryById(categoryId))
        {
            std::cout << "Category deleted successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to delete category. Category may have products or not exist." << std::endl;
        }
    }

    void executeAddProduct()
    {
        if (getStrLength(argsBuffer) == 0)
        {
            std::cout << "Usage: add-product <product_type>" << std::endl;
            std::cout << "Product types: product_by_unit, product_by_weight" << std::endl;
            return;
        }

        char productName[100], categoryName[100];
        char priceStr[20], quantityStr[20];

        std::cout << "Enter product name: ";
        std::cin.getline(productName, sizeof(productName));

        std::cout << "Enter product category: ";
        std::cin.getline(categoryName, sizeof(categoryName));

        std::cout << "Enter price per " << (strCompare(argsBuffer, "product_by_unit") ? "unit" : "kg") << ": ";
        std::cin.getline(priceStr, sizeof(priceStr));

        std::cout << "Enter quantity (" << (strCompare(argsBuffer, "product_by_unit") ? "units" : "kg") << "): ";
        std::cin.getline(quantityStr, sizeof(quantityStr));

        if (!isValidProductName(productName) || !isValidPrice(priceStr) || !isValidFloat(quantityStr))
        {
            std::cout << "Invalid input format." << std::endl;
            return;
        }

        Category* category = productMgr.findCategoryByName(categoryName);
        if (!category)
        {
            std::cout << "Category not found. Creating new category..." << std::endl;
            int newCatId = Category::generateNewCategoryId();
            category = new Category(newCatId, categoryName, "Auto-created category");
            if (!productMgr.addCategory(category))
            {
                std::cout << "Failed to create category." << std::endl;
                delete category;
                return;
            }
        }

        double price = 0.0, quantity = 0.0;
        for (int i = 0; priceStr[i] != '\0'; i++)
        {
            if (priceStr[i] == '.')
            {
                double decimal = 0.1;
                for (int j = i + 1; priceStr[j] != '\0'; j++)
                {
                    price += (priceStr[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            price = price * 10 + (priceStr[i] - '0');
        }

        for (int i = 0; quantityStr[i] != '\0'; i++)
        {
            if (quantityStr[i] == '.')
            {
                double decimal = 0.1;
                for (int j = i + 1; quantityStr[j] != '\0'; j++)
                {
                    quantity += (quantityStr[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            quantity = quantity * 10 + (quantityStr[i] - '0');
        }

        Product* newProduct = nullptr;
        int newProductId = Product::generateNewProductId();

        if (strCompare(argsBuffer, "product_by_unit"))
        {
            newProduct = new ProductByUnit(newProductId, productName, category->getId(), price, (int)quantity);
        }
        else if (strCompare(argsBuffer, "product_by_weight"))
        {
            newProduct = new ProductByWeight(newProductId, productName, category->getId(), price, quantity);
        }
        else
        {
            std::cout << "Invalid product type. Use 'product_by_unit' or 'product_by_weight'." << std::endl;
            return;
        }

        if (productMgr.addProduct(newProduct))
        {
            std::cout << "Product \"" << productName << "\" added successfully under category \"" << categoryName << "\"" << std::endl;
        }
        else
        {
            std::cout << "Failed to add product." << std::endl;
            delete newProduct;
        }
    }

    void executeDeleteProduct()
    {
        if (!isValidNumber(argsBuffer))
        {
            std::cout << "Usage: delete-product <product_id>" << std::endl;
            return;
        }

        int productId = 0;
        for (int i = 0; argsBuffer[i] != '\0'; i++) {
            productId = productId * 10 + (argsBuffer[i] - '0');
        }

        if (productMgr.removeProductById(productId))
        {
            std::cout << "Product deleted successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to delete product. Product may not exist." << std::endl;
        }
    }

    void executeLoadProducts()
    {
        if (getStrLength(argsBuffer) == 0)
        {
            std::cout << "Usage: load-products <filename>" << std::endl;
            return;
        }

        productMgr.loadProductsFromReceipt(argsBuffer);
    }


    void executeLoadGiftCards()
    {
        if (getStrLength(argsBuffer) == 0)
        {
            std::cout << "Usage: load-gift-cards <filename>" << std::endl;
            return;
        }

        std::cout << "Loading gift cards from " << argsBuffer << "..." << std::endl;

        clearAllVouchers();

        loadAllVouchers(argsBuffer);
    }

    void executeCreateSingleVoucher()
    {
        if (!auth.isManager())
        {
            std::cout << "Only managers can create vouchers." << std::endl;
            return;
        }

        char categoryIdStr[20], percentageStr[20];
        if (!parseIdCodeArgs(argsBuffer, categoryIdStr, percentageStr))
        {
            std::cout << "Usage: create-single-voucher <category_id> <percentage>" << std::endl;
            return;
        }

        if (!isValidNumber(categoryIdStr) || !isValidFloat(percentageStr))
        {
            std::cout << "Invalid format. Use numbers for category ID and percentage." << std::endl;
            return;
        }

        int categoryId = 0;
        for (int i = 0; categoryIdStr[i] != '\0'; i++)
        {
            categoryId = categoryId * 10 + (categoryIdStr[i] - '0');
        }

        double percentage = 0.0;
        for (int i = 0; percentageStr[i] != '\0'; i++)
        {
            if (percentageStr[i] == '.')
            {
                double decimal = 0.1;
                for (int j = i + 1; percentageStr[j] != '\0'; j++)
                {
                    percentage += (percentageStr[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            percentage = percentage * 10 + (percentageStr[i] - '0');
        }

        if (percentage <= 0 || percentage > 50)
        {
            std::cout << "Percentage must be between 0.1 and 50." << std::endl;
            return;
        }

        Category* category = productMgr.findCategoryById(categoryId);
        if (!category)
        {
            std::cout << "Category with ID " << categoryId << " not found." << std::endl;
            return;
        }

        if (singleVoucherCount >= voucherCapacity)
        {
            std::cout << "Maximum voucher limit reached." << std::endl;
            return;
        }

        SingleCategoryGiftCard* voucher = GiftCardFactory::createSingleCategory(categoryId, percentage);
        if (voucher)
        {
            singleVouchers[singleVoucherCount] = voucher;
            singleVoucherCount++;

            std::cout << "Single category voucher created!" << std::endl;
            std::cout << "Code: " << voucher->getCode() << std::endl;
            std::cout << "Category: " << category->getName() << std::endl;
            std::cout << "Discount: " << percentage << "%" << std::endl;

            saveAllVouchers("vouchers.txt");
        }
        else {
            std::cout << "Failed to create voucher." << std::endl;
        }
    }


    void executeCreateMultipleVoucher()
    {
        if (!auth.isManager())
        {
            std::cout << "Only managers can create vouchers." << std::endl;
            return;
        }

        char categoriesStr[100], percentageStr[20];
        if (!parseIdCodeArgs(argsBuffer, categoriesStr, percentageStr))
        {
            std::cout << "Usage: create-multiple-voucher <cat1,cat2,cat3> <percentage>" << std::endl;
            return;
        }

        if (!isValidFloat(percentageStr)) 
        {
            std::cout << "Invalid percentage format." << std::endl;
            return;
        }

        int categoryIds[10];
        int categoryCount = 0;

        char temp[100];
        strCopy(temp, categoriesStr, sizeof(temp));

        int start = 0;
        for (int i = 0; i <= getStrLength(temp) && categoryCount < 10; i++)
        {
            if (temp[i] == ',' || temp[i] == '\0') 
            {
                temp[i] = '\0';

                if (isValidNumber(&temp[start]))
                {
                    int catId = 0;
                    for (int j = start; temp[j] != '\0'; j++)
                    {
                        catId = catId * 10 + (temp[j] - '0');
                    }
                    categoryIds[categoryCount] = catId;
                    categoryCount++;
                }
                start = i + 1;
            }
        }

        if (categoryCount == 0) 
        {
            std::cout << "No valid category IDs found. Use format: 1,2,3" << std::endl;
            return;
        }

        double percentage = 0.0;
        for (int i = 0; percentageStr[i] != '\0'; i++) 
        {
            if (percentageStr[i] == '.')
            {
                double decimal = 0.1;
                for (int j = i + 1; percentageStr[j] != '\0'; j++) 
                {
                    percentage += (percentageStr[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            percentage = percentage * 10 + (percentageStr[i] - '0');
        }

        if (percentage <= 0 || percentage > 50) 
        {
            std::cout << "Percentage must be between 0.1 and 50." << std::endl;
            return;
        }

        if (multipleVoucherCount >= voucherCapacity)
        {
            std::cout << "Maximum voucher limit reached." << std::endl;
            return;
        }

        MultipleCategoryGiftCard* voucher = GiftCardFactory::createMultipleCategory(categoryIds, categoryCount, percentage);
        if (voucher)
        {
            multipleVouchers[multipleVoucherCount] = voucher;
            multipleVoucherCount++;

            std::cout << "Multiple category voucher created!" << std::endl;
            std::cout << "Code: " << voucher->getCode() << std::endl;
            std::cout << "Categories: " << categoryCount << " selected" << std::endl;
            std::cout << "Discount: " << percentage << "%" << std::endl;
        }
        else 
        {
            std::cout << "Failed to create voucher." << std::endl;
        }


        if (voucher)
        {
            multipleVouchers[multipleVoucherCount] = voucher;
            multipleVoucherCount++;

            std::cout << "Multiple category voucher created!" << std::endl;
            std::cout << "Code: " << voucher->getCode() << std::endl;
            std::cout << "Categories: " << categoryCount << " selected" << std::endl;
            std::cout << "Discount: " << percentage << "%" << std::endl;

            saveAllVouchers("vouchers.txt");
        }
        else
        {
            std::cout << "Failed to create voucher." << std::endl;
        }



    }

    void executeCreateAllVoucher()
    {
        if (!auth.isManager())
        {
            std::cout << "Only managers can create vouchers." << std::endl;
            return;
        }

        if (getStrLength(argsBuffer) == 0)
        {
            std::cout << "Usage: create-all-voucher <percentage>" << std::endl;
            return;
        }

        if (!isValidFloat(argsBuffer)) 
        {
            std::cout << "Invalid percentage format." << std::endl;
            return;
        }

        double percentage = 0.0;
        for (int i = 0; argsBuffer[i] != '\0'; i++) 
        {
            if (argsBuffer[i] == '.') 
            {
                double decimal = 0.1;
                for (int j = i + 1; argsBuffer[j] != '\0'; j++)
                {
                    percentage += (argsBuffer[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            percentage = percentage * 10 + (argsBuffer[i] - '0');
        }

        if (percentage <= 0 || percentage > 50)
        {
            std::cout << "Percentage must be between 0.1 and 50." << std::endl;
            return;
        }

        if (allVoucherCount >= voucherCapacity)
        {
            std::cout << "Maximum voucher limit reached." << std::endl;
            return;
        }

        AllProductsGiftCard* voucher = GiftCardFactory::createAllProducts(percentage);
        if (voucher) 
        {
            allVouchers[allVoucherCount] = voucher;
            allVoucherCount++;

            std::cout << "All products voucher created!" << std::endl;
            std::cout << "Code: " << voucher->getCode() << std::endl;
            std::cout << "Applies to: All products" << std::endl;
            std::cout << "Discount: " << percentage << "%" << std::endl;
        }
        else
        {
            std::cout << "Failed to create voucher." << std::endl;
        }


        if (voucher)
        {
            allVouchers[allVoucherCount] = voucher;
            allVoucherCount++;

            std::cout << "All products voucher created!" << std::endl;
            std::cout << "Code: " << voucher->getCode() << std::endl;
            std::cout << "Applies to: All products" << std::endl;
            std::cout << "Discount: " << percentage << "%" << std::endl;

            saveAllVouchers("vouchers.txt");
        }
        else
        {
            std::cout << "Failed to create voucher." << std::endl;
        }


    }

    void executeListVouchers()
    {
        int totalVouchers = singleVoucherCount + multipleVoucherCount + allVoucherCount;

        if (totalVouchers == 0) 
        {
            std::cout << "No vouchers available." << std::endl;
            return;
        }

        std::cout << "Available Vouchers:" << std::endl;
        std::cout << "==================" << std::endl;

        for (int i = 0; i < singleVoucherCount; i++)
        {
            SingleCategoryGiftCard* voucher = singleVouchers[i];
            Category* category = productMgr.findCategoryById(voucher->getCategoryId());

            std::cout << "Code: " << voucher->getCode()
                << " | Type: Single Category"
                << " | Category: " << (category ? category->getName() : "Unknown")
                << " | Discount: " << voucher->getDiscountPercentage() << "%"
                << " | Used: " << (voucher->isUsed() ? "Yes" : "No") << std::endl;
        }

        for (int i = 0; i < multipleVoucherCount; i++)
        {
            MultipleCategoryGiftCard* voucher = multipleVouchers[i];

            std::cout << "Code: " << voucher->getCode()
                << " | Type: Multiple Categories"
                << " | Categories: " << voucher->getCategoryCount()
                << " | Discount: " << voucher->getDiscountPercentage() << "%"
                << " | Used: " << (voucher->isUsed() ? "Yes" : "No") << std::endl;
        }

        for (int i = 0; i < allVoucherCount; i++)
        {
            AllProductsGiftCard* voucher = allVouchers[i];

            std::cout << "Code: " << voucher->getCode()
                << " | Type: All Products"
                << " | Discount: " << voucher->getDiscountPercentage() << "%"
                << " | Used: " << (voucher->isUsed() ? "Yes" : "No") << std::endl;
        }

        std::cout << "==================" << std::endl;
        std::cout << "Total vouchers: " << totalVouchers << std::endl;
    }

    void executeApprove()
    {
        char cashierIdStr[20], specialCode[10];
        if (!parseIdCodeArgs(argsBuffer, cashierIdStr, specialCode))
        {
            std::cout << "Usage: approve <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashierIdStr))
        {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(specialCode))
        {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashierId = 0;
        for (int i = 0; cashierIdStr[i] != '\0'; i++)
        {
            cashierId = cashierId * 10 + (cashierIdStr[i] - '0');
        }

        if (workerMgr.approveWorker(cashierId, auth.getCurrentUserId(), specialCode))
        {
            std::cout << "Cashier approved successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to approve cashier. Cashier may not exist or already be approved." << std::endl;
        }
    }

    void executeDecline()
    {
        char cashierIdStr[20], specialCode[10];
        if (!parseIdCodeArgs(argsBuffer, cashierIdStr, specialCode))
        {
            std::cout << "Usage: decline <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashierIdStr))
        {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(specialCode))
        {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashierId = 0;
        for (int i = 0; cashierIdStr[i] != '\0'; i++)
        {
            cashierId = cashierId * 10 + (cashierIdStr[i] - '0');
        }

        Worker* worker = workerMgr.findWorkerById(cashierId);
        if (!worker || !strCompare(worker->getRole(), "Cashier"))
        {
            std::cout << "Pending cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        if (cashier->isApproved())
        {
            std::cout << "Cashier is already approved." << std::endl;
            return;
        }

        if (workerMgr.removeWorker(cashierId))
        {
            std::cout << "Cashier registration declined and removed." << std::endl;
        }
        else
        {
            std::cout << "Failed to decline cashier registration." << std::endl;
        }
    }

    void executeWarnCashier()
    {
        char cashierIdStr[20], pointsStr[10];
        if (!parseIdPointsArgs(argsBuffer, cashierIdStr, pointsStr))
        {
            std::cout << "Usage: warn-cashier <cashier_id> <points>" << std::endl;
            std::cout << "Points: 100 (low), 200 (medium), 300 (high)" << std::endl;
            return;
        }

        if (!isValidNumber(cashierIdStr) || !isValidNumber(pointsStr))
        {
            std::cout << "Invalid number format." << std::endl;
            return;
        }

        int cashierId = 0, points = 0;
        for (int i = 0; cashierIdStr[i] != '\0'; i++)
        {
            cashierId = cashierId * 10 + (cashierIdStr[i] - '0');
        }
        for (int i = 0; pointsStr[i] != '\0'; i++)
        {
            points = points * 10 + (pointsStr[i] - '0');
        }

        if (points != 100 && points != 200 && points != 300)
        {
            std::cout << "Invalid warning points. Use 100, 200, or 300." << std::endl;
            return;
        }

        Worker* worker = workerMgr.findWorkerById(cashierId);
        if (!worker || !strCompare(worker->getRole(), "Cashier"))
        {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        const char* reason = "Disciplinary action by manager";
        cashier->addWarning(points, reason);

        std::cout << "Warning added to cashier " << cashierId << " (" << points << " points)" << std::endl;
    }

    void executePromoteCashier()
    {
        char cashierIdStr[20], specialCode[10];
        if (!parseIdCodeArgs(argsBuffer, cashierIdStr, specialCode))
        {
            std::cout << "Usage: promote-cashier <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashierIdStr))
        {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(specialCode))
        {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashierId = 0;
        for (int i = 0; cashierIdStr[i] != '\0'; i++)
        {
            cashierId = cashierId * 10 + (cashierIdStr[i] - '0');
        }

        Worker* worker = workerMgr.findWorkerById(cashierId);
        if (!worker || !strCompare(worker->getRole(), "Cashier"))
        {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        Manager* manager = static_cast<Manager*>(auth.getCurrentUser());

        if (manager->promoteCashier(cashier, specialCode))
        {
            std::cout << "Cashier promoted successfully!" << std::endl;
        }
        else
        {
            std::cout << "Failed to promote cashier. Check requirements (50+ transactions, <100 warning points)." << std::endl;
        }
    }

    void executeFireCashier()
    {
        char cashierIdStr[20], specialCode[10];
        if (!parseIdCodeArgs(argsBuffer, cashierIdStr, specialCode))
        {
            std::cout << "Usage: fire-cashier <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashierIdStr))
        {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(specialCode))
        {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashierId = 0;
        for (int i = 0; cashierIdStr[i] != '\0'; i++)
        {
            cashierId = cashierId * 10 + (cashierIdStr[i] - '0');
        }

        Worker* worker = workerMgr.findWorkerById(cashierId);
        if (!worker || !strCompare(worker->getRole(), "Cashier"))
        {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        Manager* manager = static_cast<Manager*>(auth.getCurrentUser());

        if (manager->fireCashier(cashier, specialCode))
        {
            std::cout << "Cashier fired successfully!" << std::endl;
            workerMgr.removeWorker(cashierId);
        }
        else
        {
            std::cout << "Failed to fire cashier." << std::endl;
        }
    }

    void executeSell()
    {
        if (!auth.isCashier())
        {
            std::cout << "Only cashiers can perform sales." << std::endl;
            return;
        }

        transactionMgr.processSellCommand(auth.getCurrentUserId());
    }

    bool parseLoginArgs(const char* args, char* idStr, char* password)
    {
        int spacePos = -1;
        for (int i = 0; args[i] != '\0'; i++)
        {
            if (isWhitespace(args[i]))
            {
                spacePos = i;
                break;
            }
        }

        if (spacePos == -1) return false;

        for (int i = 0; i < spacePos && i < 19; i++)
        {
            idStr[i] = args[i];
        }
        idStr[spacePos < 19 ? spacePos : 19] = '\0';

        int pwdStart = spacePos;
        while (args[pwdStart] != '\0' && isWhitespace(args[pwdStart]))
        {
            pwdStart++;
        }

        strCopy(password, &args[pwdStart], 100);

        return getStrLength(idStr) > 0 && getStrLength(password) > 0;
    }

    bool parseCategoryArgs(const char* args, char* name, char* description)
    {
        int spacePos = -1;
        for (int i = 0; args[i] != '\0'; i++)
        {
            if (isWhitespace(args[i]))
            {
                spacePos = i;
                break;
            }
        }

        if (spacePos == -1) return false;

        for (int i = 0; i < spacePos && i < 99; i++)
        {
            name[i] = args[i];
        }
        name[spacePos < 99 ? spacePos : 99] = '\0';

        int descStart = spacePos;
        while (args[descStart] != '\0' && isWhitespace(args[descStart]))
        {
            descStart++;
        }

        strCopy(description, &args[descStart], 300);

        return getStrLength(name) > 0 && getStrLength(description) > 0;
    }

    bool parseIdCodeArgs(const char* args, char* idStr, char* code)
    {
        int spacePos = -1;
        for (int i = 0; args[i] != '\0'; i++)
        {
            if (isWhitespace(args[i]))
            {
                spacePos = i;
                break;
            }
        }

        if (spacePos == -1) return false;

        for (int i = 0; i < spacePos && i < 19; i++)
        {
            idStr[i] = args[i];
        }
        idStr[spacePos < 19 ? spacePos : 19] = '\0';

        int codeStart = spacePos;
        while (args[codeStart] != '\0' && isWhitespace(args[codeStart]))
        {
            codeStart++;
        }

        strCopy(code, &args[codeStart], 10);

        return getStrLength(idStr) > 0 && getStrLength(code) > 0;
    }

    bool parseIdPointsArgs(const char* args, char* idStr, char* pointsStr)
    {
        return parseIdCodeArgs(args, idStr, pointsStr);
    }

    double calculateVoucherDiscount(const char* voucherCode, double originalPrice, int productCategoryId)
    {
        if (!voucherCode || getStrLength(voucherCode) == 0)
        {
            return 0.0;
        }

        for (int i = 0; i < singleVoucherCount; i++) 
        {
            SingleCategoryGiftCard* voucher = singleVouchers[i];
            if (strCompare(voucher->getCode(), voucherCode) && !voucher->isUsed()) 
            {
                if (voucher->canApplyToCategory(productCategoryId))
                {
                    voucher->markAsUsed();
                    return voucher->calculateDiscount(originalPrice, productCategoryId);
                }
            }
        }

        for (int i = 0; i < multipleVoucherCount; i++) 
        {
            MultipleCategoryGiftCard* voucher = multipleVouchers[i];
            if (strCompare(voucher->getCode(), voucherCode) && !voucher->isUsed())
            {
                if (voucher->canApplyToCategory(productCategoryId))
                {
                    voucher->markAsUsed();
                    return voucher->calculateDiscount(originalPrice, productCategoryId);
                }
            }
        }

        for (int i = 0; i < allVoucherCount; i++)
        {
            AllProductsGiftCard* voucher = allVouchers[i];
            if (strCompare(voucher->getCode(), voucherCode) && !voucher->isUsed()) 
            {
                voucher->markAsUsed();
                return voucher->calculateDiscount(originalPrice, productCategoryId);
            }
        }

        return 0.0;
    }

    bool isValidVoucherCode(const char* code)
    {
        if (!code || getStrLength(code) == 0) 
        {
            return false;
        }

        for (int i = 0; i < singleVoucherCount; i++)
        {
            if (strCompare(singleVouchers[i]->getCode(), code))
            {
                return true;
            }
        }

        for (int i = 0; i < multipleVoucherCount; i++) 
        {
            if (strCompare(multipleVouchers[i]->getCode(), code))
            {
                return true;
            }
        }

        for (int i = 0; i < allVoucherCount; i++)
        {
            if (strCompare(allVouchers[i]->getCode(), code))
            {
                return true;
            }
        }

        return false;
    }
};

int main()
{
    std::cout << "Initializing FMI Supermarket Management System..." << std::endl;

    try
    {
        CLISystem system;
        system.run();
    }
    catch (...)
    {
        std::cout << "System error occurred. Shutting down safely..." << std::endl;
        return 1;
    }

    return 0;
}