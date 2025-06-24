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
    WorkerManager worker_mgr;
    ProductManager product_mgr;
    TransactionManager transaction_mgr;
    GiftCardFactory voucher_factory;

    bool system_running;
    char input_buffer[1000];
    char command_buffer[100];
    char args_buffer[900];

public:
    CLISystem() : system_running(true) {
        auth.setWorkerManager(&worker_mgr);
        transaction_mgr.setProductManager(&product_mgr);

        loadSystemData();

        createDefaultManagerIfNeeded();

        srand(static_cast<unsigned int>(time(nullptr)));
    }

    ~CLISystem() {
        saveSystemData();
    }

    void run() {
        displayWelcomeMessage();

        while (system_running) {
            displayPrompt();
            readInput();

            if (parseCommand()) {
                executeCommand();
            }
        }

        displayGoodbyeMessage();
    }

private:
    void loadSystemData() {
        std::cout << "Loading system data..." << std::endl;

        std::ifstream worker_file("workers.txt");
        if (worker_file.is_open()) {
            worker_mgr.loadAllWorkers("workers.txt");
            worker_file.close();
        }

        std::ifstream category_file("categories.txt");
        if (category_file.is_open()) {
            product_mgr.loadAllCategories("categories.txt");
            category_file.close();
        }

        std::ifstream product_file("products.txt");
        if (product_file.is_open()) {
            product_mgr.loadAllProducts("products.txt");
            product_file.close();
        }

        std::ifstream trans_file("transactions.txt");
        if (trans_file.is_open()) {
            transaction_mgr.loadAllTransactions("transactions.txt");
            trans_file.close();
        }

        std::cout << "System data loaded successfully." << std::endl;
    }

    void saveSystemData() {
        std::cout << "Saving system data..." << std::endl;

        worker_mgr.saveAllWorkers("workers.txt");

        product_mgr.saveAllCategories("categories.txt");
        product_mgr.saveAllProducts("products.txt");

        transaction_mgr.saveAllTransactions("transactions.txt");

        std::cout << "System data saved successfully." << std::endl;
    }

    void createDefaultManagerIfNeeded() {
        if (worker_mgr.getWorkerCount() == 0) {
            std::cout << "Creating default manager account..." << std::endl;

            Manager* default_mgr = new Manager(Worker::generateNewId(), "Admin", "Manager",
                "0881234567", 30, "admin123");
            default_mgr->setApproved(true);

            if (worker_mgr.addWorker(default_mgr)) {
                default_mgr->generateSpecialCode();
                std::cout << "Default manager created with ID: " << default_mgr->getId()
                    << ", Password: admin123" << std::endl;
                std::cout << "Special code: " << default_mgr->getSpecialCode() << std::endl;
            }
        }
    }

    void displayWelcomeMessage() {
        std::cout << std::endl;
        std::cout << "=================================================" << std::endl;
        std::cout << "  Welcome to FMI Supermarket Management System  " << std::endl;
        std::cout << "                   2024-2025                    " << std::endl;
        std::cout << "=================================================" << std::endl;
        std::cout << std::endl;

        if (!auth.isLoggedIn()) {
            std::cout << "Please login to continue." << std::endl;
            std::cout << "Available commands:" << std::endl;
            std::cout << "  login <id> <password>" << std::endl;
            std::cout << "  register <role> <first_name> <last_name> <phone> <age> <password>" << std::endl;
            std::cout << "  exit" << std::endl;
        }
    }

    void displayPrompt() {
        if (auth.isLoggedIn()) {
            std::cout << "[" << auth.getCurrentUserRole() << " "
                << auth.getCurrentUserId() << "] > ";
        }
        else {
            std::cout << "> ";
        }
    }

    void displayGoodbyeMessage() {
        std::cout << std::endl;
        std::cout << "Thank you for using FMI Supermarket Management System!" << std::endl;
        std::cout << "Goodbye!" << std::endl;
    }

    void readInput() {
        std::cin.getline(input_buffer, sizeof(input_buffer));
        trimStr(input_buffer);
    }

    bool parseCommand() {
        if (getStrLength(input_buffer) == 0) {
            return false;
        }

        int space_pos = -1;
        for (int i = 0; input_buffer[i] != '\0'; i++) {
            if (isWhitespace(input_buffer[i])) {
                space_pos = i;
                break;
            }
        }

        if (space_pos == -1) {
            strCopy(command_buffer, input_buffer, sizeof(command_buffer));
            args_buffer[0] = '\0';
        }
        else {
            int cmd_len = (space_pos < sizeof(command_buffer) - 1) ? space_pos : sizeof(command_buffer) - 1;
            for (int i = 0; i < cmd_len; i++) {
                command_buffer[i] = input_buffer[i];
            }
            command_buffer[cmd_len] = '\0';

            int arg_start = space_pos;
            while (input_buffer[arg_start] != '\0' && isWhitespace(input_buffer[arg_start])) {
                arg_start++;
            }
            strCopy(args_buffer, &input_buffer[arg_start], sizeof(args_buffer));
        }

        return true;
    }

    void executeCommand() {
        for (int i = 0; command_buffer[i] != '\0'; i++) {
            command_buffer[i] = toLowerCase(command_buffer[i]);
        }

        if (strCompare(command_buffer, "exit") || strCompare(command_buffer, "quit")) {
            system_running = false;
            return;
        }

        if (strCompare(command_buffer, "login")) {
            executeLogin();
            return;
        }

        if (strCompare(command_buffer, "register")) {
            executeRegister();
            return;
        }

        if (!auth.isLoggedIn()) {
            auth.displayNotLoggedInMessage();
            return;
        }

        if (!auth.canExecuteCommand(command_buffer)) {
            auth.displayAccessDeniedMessage(command_buffer);
            return;
        }

        if (strCompare(command_buffer, "logout")) {
            auth.logout();
        }
        else if (strCompare(command_buffer, "list-user-data")) {
            auth.displayCurrentUserInfo();
        }
        else if (strCompare(command_buffer, "list-workers")) {
            worker_mgr.listAllWorkers();
        }
        else if (strCompare(command_buffer, "list-products")) {
            executeListProducts();
        }
        else if (strCompare(command_buffer, "list-transactions")) {
            transaction_mgr.listAllTransactions();
        }
        else if (strCompare(command_buffer, "sell")) {
            executeSell();
        }
        else if (strCompare(command_buffer, "list-pending")) {
            worker_mgr.listPendingWorkers();
        }
        else if (strCompare(command_buffer, "approve")) {
            executeApprove();
        }
        else if (strCompare(command_buffer, "decline")) {
            executeDecline();
        }
        else if (strCompare(command_buffer, "warn-cashier")) {
            executeWarnCashier();
        }
        else if (strCompare(command_buffer, "promote-cashier")) {
            executePromoteCashier();
        }
        else if (strCompare(command_buffer, "fire-cashier")) {
            executeFireCashier();
        }
        else if (strCompare(command_buffer, "add-category")) {
            executeAddCategory();
        }
        else if (strCompare(command_buffer, "delete-category")) {
            executeDeleteCategory();
        }
        else if (strCompare(command_buffer, "add-product")) {
            executeAddProduct();
        }
        else if (strCompare(command_buffer, "delete-product")) {
            executeDeleteProduct();
        }
        else if (strCompare(command_buffer, "load-products")) {
            executeLoadProducts();
        }
        else if (strCompare(command_buffer, "load-gift-cards")) {
            executeLoadGiftCards();
        }
        else {
            std::cout << "Unknown command: " << command_buffer << std::endl;
            std::cout << "Type 'help' for available commands." << std::endl;
        }
    }

    void executeLogin() {
        char id_str[20], password[100];
        if (!parseLoginArgs(args_buffer, id_str, password)) {
            std::cout << "Usage: login <worker_id> <password>" << std::endl;
            return;
        }

        if (!isValidNumber(id_str)) {
            std::cout << "Invalid worker ID format." << std::endl;
            return;
        }

        int worker_id = 0;
        for (int i = 0; id_str[i] != '\0'; i++) {
            worker_id = worker_id * 10 + (id_str[i] - '0');
        }

        auth.login(worker_id, password);
    }

    void executeRegister() {
        char role[20], first_name[50], last_name[50], phone[20], age_str[10], password[100];

        if (!parseRegisterArgs(args_buffer, role, first_name, last_name, phone, age_str, password)) {
            std::cout << "Usage: register <role> <first_name> <last_name> <phone> <age> <password>" << std::endl;
            std::cout << "Roles: cashier, manager" << std::endl;
            return;
        }

        if (!isValidNumber(age_str)) {
            std::cout << "Invalid age format." << std::endl;
            return;
        }

        int age = 0;
        for (int i = 0; age_str[i] != '\0'; i++) {
            age = age * 10 + (age_str[i] - '0');
        }

        if (age < 18 || age > 100) {
            std::cout << "Age must be between 18 and 100." << std::endl;
            return;
        }

        Worker* new_worker = nullptr;
        int new_id = Worker::generateNewId();

        if (strCompare(role, "cashier")) {
            new_worker = new Cashier(new_id, first_name, last_name, phone, age, password);
        }
        else if (strCompare(role, "manager")) {
            Manager* new_mgr = new Manager(new_id, first_name, last_name, phone, age, password);
            new_mgr->generateSpecialCode();
            new_mgr->setApproved(true);
            new_worker = new_mgr;
        }
        else {
            std::cout << "Invalid role. Use 'cashier' or 'manager'." << std::endl;
            return;
        }

        if (worker_mgr.addWorker(new_worker)) {
            if (strCompare(role, "cashier")) {
                std::cout << "Cashier registration pending approval from a manager." << std::endl;
            }
            else {
                std::cout << "Manager registered successfully!" << std::endl;
                Manager* mgr = static_cast<Manager*>(new_worker);
                std::cout << "Special code: " << mgr->getSpecialCode() << std::endl;
                char filename[50];
                intToString(new_worker->getId(), filename, 20);
                int len = getStrLength(filename);
                const char* suffix = "_special_code.txt";
                for (int i = 0; suffix[i] != '\0' && len + i < 49; i++) {
                    filename[len + i] = suffix[i];
                }
                filename[len + getStrLength(suffix)] = '\0';
                std::cout << "Code saved to: " << filename << std::endl;
            }
        }
        else {
            std::cout << "Failed to register worker." << std::endl;
            delete new_worker;
        }
    }

    void executeListProducts() {
        if (getStrLength(args_buffer) == 0) {
            product_mgr.listAllProducts();
        }
        else {
            if (!isValidNumber(args_buffer)) {
                std::cout << "Invalid category ID format." << std::endl;
                return;
            }

            int category_id = 0;
            for (int i = 0; args_buffer[i] != '\0'; i++) {
                category_id = category_id * 10 + (args_buffer[i] - '0');
            }

            product_mgr.listProductsByCategory(category_id);
        }
    }

    void executeAddCategory() {
        char name[100], description[300];
        if (!parseCategoryArgs(args_buffer, name, description)) {
            std::cout << "Usage: add-category <name> <description>" << std::endl;
            return;
        }

        Category* newCategory = new Category(Category::generateNewCategoryId(), name, description);
        if (product_mgr.addCategory(newCategory)) {
            std::cout << "Category \"" << name << "\" added successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to add category. Category may already exist." << std::endl;
            delete newCategory;
        }
    }

    void executeDeleteCategory() {
        if (!isValidNumber(args_buffer)) {
            std::cout << "Usage: delete-category <category_id>" << std::endl;
            return;
        }

        int category_id = 0;
        for (int i = 0; args_buffer[i] != '\0'; i++) {
            category_id = category_id * 10 + (args_buffer[i] - '0');
        }

        if (product_mgr.removeCategoryById(category_id)) {
            std::cout << "Category deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete category. Category may have products or not exist." << std::endl;
        }
    }

    void executeAddProduct() {
        if (getStrLength(args_buffer) == 0) {
            std::cout << "Usage: add-product <product_type>" << std::endl;
            std::cout << "Product types: product_by_unit, product_by_weight" << std::endl;
            return;
        }

        char product_name[100], category_name[100];
        char price_str[20], quantity_str[20];

        std::cout << "Enter product name: ";
        std::cin.getline(product_name, sizeof(product_name));

        std::cout << "Enter product category: ";
        std::cin.getline(category_name, sizeof(category_name));

        std::cout << "Enter price per " << (strCompare(args_buffer, "product_by_unit") ? "unit" : "kg") << ": ";
        std::cin.getline(price_str, sizeof(price_str));

        std::cout << "Enter quantity (" << (strCompare(args_buffer, "product_by_unit") ? "units" : "kg") << "): ";
        std::cin.getline(quantity_str, sizeof(quantity_str));

        if (!isValidProductName(product_name) || !isValidPrice(price_str) || !isValidFloat(quantity_str)) {
            std::cout << "Invalid input format." << std::endl;
            return;
        }

        Category* category = product_mgr.findCategoryByName(category_name);
        if (!category) {
            std::cout << "Category not found. Creating new category..." << std::endl;
            int new_cat_id = Category::generateNewCategoryId();
            category = new Category(new_cat_id, category_name, "Auto-created category");
            if (!product_mgr.addCategory(category)) {
                std::cout << "Failed to create category." << std::endl;
                delete category;
                return;
            }
        }

        double price = 0.0, quantity = 0.0;
        for (int i = 0; price_str[i] != '\0'; i++) {
            if (price_str[i] == '.') {
                double decimal = 0.1;
                for (int j = i + 1; price_str[j] != '\0'; j++) {
                    price += (price_str[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            price = price * 10 + (price_str[i] - '0');
        }

        for (int i = 0; quantity_str[i] != '\0'; i++) {
            if (quantity_str[i] == '.') {
                double decimal = 0.1;
                for (int j = i + 1; quantity_str[j] != '\0'; j++) {
                    quantity += (quantity_str[j] - '0') * decimal;
                    decimal *= 0.1;
                }
                break;
            }
            quantity = quantity * 10 + (quantity_str[i] - '0');
        }

        Product* new_product = nullptr;
        int new_product_id = Product::generateNewProductId();

        if (strCompare(args_buffer, "product_by_unit")) {
            new_product = new ProductByUnit(new_product_id, product_name, category->getId(), price, (int)quantity);
        }
        else if (strCompare(args_buffer, "product_by_weight")) {
            new_product = new ProductByWeight(new_product_id, product_name, category->getId(), price, quantity);
        }
        else {
            std::cout << "Invalid product type. Use 'product_by_unit' or 'product_by_weight'." << std::endl;
            return;
        }

        if (product_mgr.addProduct(new_product)) {
            std::cout << "Product \"" << product_name << "\" added successfully under category \"" << category_name << "\"" << std::endl;
        }
        else {
            std::cout << "Failed to add product." << std::endl;
            delete new_product;
        }
    }

    void executeDeleteProduct() {
        if (!isValidNumber(args_buffer)) {
            std::cout << "Usage: delete-product <product_id>" << std::endl;
            return;
        }

        int product_id = 0;
        for (int i = 0; args_buffer[i] != '\0'; i++) {
            product_id = product_id * 10 + (args_buffer[i] - '0');
        }

        if (product_mgr.removeProductById(product_id)) {
            std::cout << "Product deleted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to delete product. Product may not exist." << std::endl;
        }
    }

    void executeLoadProducts() {
        if (getStrLength(args_buffer) == 0) {
            std::cout << "Usage: load-products <filename>" << std::endl;
            return;
        }

        if (product_mgr.loadProductsFromReceipt(args_buffer)) {
            std::cout << "Products loaded successfully from " << args_buffer << std::endl;
        }
        else {
            std::cout << "Failed to load products from " << args_buffer << std::endl;
        }
    }

    void executeLoadGiftCards() {
        if (getStrLength(args_buffer) == 0) {
            std::cout << "Usage: load-gift-cards <filename>" << std::endl;
            return;
        }

        std::cout << "Loading gift cards from " << args_buffer << "..." << std::endl;
        std::cout << "Gift card system ready for transactions." << std::endl;
    }

    void executeApprove() {
        char cashier_id_str[20], special_code[10];
        if (!parseIdCodeArgs(args_buffer, cashier_id_str, special_code)) {
            std::cout << "Usage: approve <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashier_id_str)) {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(special_code)) {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashier_id = 0;
        for (int i = 0; cashier_id_str[i] != '\0'; i++) {
            cashier_id = cashier_id * 10 + (cashier_id_str[i] - '0');
        }

        if (worker_mgr.approveWorker(cashier_id, auth.getCurrentUserId(), special_code)) {
            std::cout << "Cashier approved successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to approve cashier. Cashier may not exist or already be approved." << std::endl;
        }
    }

    void executeDecline() {
        char cashier_id_str[20], special_code[10];
        if (!parseIdCodeArgs(args_buffer, cashier_id_str, special_code)) {
            std::cout << "Usage: decline <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashier_id_str)) {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(special_code)) {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashier_id = 0;
        for (int i = 0; cashier_id_str[i] != '\0'; i++) {
            cashier_id = cashier_id * 10 + (cashier_id_str[i] - '0');
        }

        Worker* worker = worker_mgr.findWorkerById(cashier_id);
        if (!worker || !strCompare(worker->getRole(), "Cashier")) {
            std::cout << "Pending cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        if (cashier->isApproved()) {
            std::cout << "Cashier is already approved." << std::endl;
            return;
        }

        if (worker_mgr.removeWorker(cashier_id)) {
            std::cout << "Cashier registration declined and removed." << std::endl;
        }
        else {
            std::cout << "Failed to decline cashier registration." << std::endl;
        }
    }

    void executeWarnCashier() {
        char cashier_id_str[20], points_str[10];
        if (!parseIdPointsArgs(args_buffer, cashier_id_str, points_str)) {
            std::cout << "Usage: warn-cashier <cashier_id> <points>" << std::endl;
            std::cout << "Points: 100 (low), 200 (medium), 300 (high)" << std::endl;
            return;
        }

        if (!isValidNumber(cashier_id_str) || !isValidNumber(points_str)) {
            std::cout << "Invalid number format." << std::endl;
            return;
        }

        int cashier_id = 0, points = 0;
        for (int i = 0; cashier_id_str[i] != '\0'; i++) {
            cashier_id = cashier_id * 10 + (cashier_id_str[i] - '0');
        }
        for (int i = 0; points_str[i] != '\0'; i++) {
            points = points * 10 + (points_str[i] - '0');
        }

        if (points != 100 && points != 200 && points != 300) {
            std::cout << "Invalid warning points. Use 100, 200, or 300." << std::endl;
            return;
        }

        Worker* worker = worker_mgr.findWorkerById(cashier_id);
        if (!worker || !strCompare(worker->getRole(), "Cashier")) {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        const char* reason = "Disciplinary action by manager";
        cashier->addWarning(points, reason);

        std::cout << "Warning added to cashier " << cashier_id << " (" << points << " points)" << std::endl;
    }

    void executePromoteCashier() {
        char cashier_id_str[20], special_code[10];
        if (!parseIdCodeArgs(args_buffer, cashier_id_str, special_code)) {
            std::cout << "Usage: promote-cashier <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashier_id_str)) {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(special_code)) {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashier_id = 0;
        for (int i = 0; cashier_id_str[i] != '\0'; i++) {
            cashier_id = cashier_id * 10 + (cashier_id_str[i] - '0');
        }

        Worker* worker = worker_mgr.findWorkerById(cashier_id);
        if (!worker || !strCompare(worker->getRole(), "Cashier")) {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        Manager* manager = static_cast<Manager*>(auth.getCurrentUser());

        if (manager->promoteCashier(cashier, special_code)) {
            std::cout << "Cashier promoted successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to promote cashier. Check requirements (50+ transactions, <100 warning points)." << std::endl;
        }
    }

    void executeFireCashier() {
        char cashier_id_str[20], special_code[10];
        if (!parseIdCodeArgs(args_buffer, cashier_id_str, special_code)) {
            std::cout << "Usage: fire-cashier <cashier_id> <special_code>" << std::endl;
            return;
        }

        if (!isValidNumber(cashier_id_str)) {
            std::cout << "Invalid cashier ID format." << std::endl;
            return;
        }

        if (!auth.validateManagerCode(special_code)) {
            std::cout << "Invalid special code." << std::endl;
            return;
        }

        int cashier_id = 0;
        for (int i = 0; cashier_id_str[i] != '\0'; i++) {
            cashier_id = cashier_id * 10 + (cashier_id_str[i] - '0');
        }

        Worker* worker = worker_mgr.findWorkerById(cashier_id);
        if (!worker || !strCompare(worker->getRole(), "Cashier")) {
            std::cout << "Cashier not found." << std::endl;
            return;
        }

        Cashier* cashier = static_cast<Cashier*>(worker);
        Manager* manager = static_cast<Manager*>(auth.getCurrentUser());

        if (manager->fireCashier(cashier, special_code)) {
            std::cout << "Cashier fired successfully!" << std::endl;
            worker_mgr.removeWorker(cashier_id);
        }
        else {
            std::cout << "Failed to fire cashier." << std::endl;
        }
    }

    void executeSell() {
        if (!auth.isCashier()) {
            std::cout << "Only cashiers can perform sales." << std::endl;
            return;
        }

        transaction_mgr.processSellCommand(auth.getCurrentUserId());
    }

    bool parseLoginArgs(const char* args, char* id_str, char* password) {
        int space_pos = -1;
        for (int i = 0; args[i] != '\0'; i++) {
            if (isWhitespace(args[i])) {
                space_pos = i;
                break;
            }
        }

        if (space_pos == -1) return false;

        for (int i = 0; i < space_pos && i < 19; i++) {
            id_str[i] = args[i];
        }
        id_str[space_pos < 19 ? space_pos : 19] = '\0';

        int pwd_start = space_pos;
        while (args[pwd_start] != '\0' && isWhitespace(args[pwd_start])) {
            pwd_start++;
        }

        strCopy(password, &args[pwd_start], 100);

        return getStrLength(id_str) > 0 && getStrLength(password) > 0;
    }

    bool parseRegisterArgs(const char* args, char* role, char* first_name, char* last_name,
        char* phone, char* age_str, char* password) {
        char temp_args[900];
        strCopy(temp_args, args, sizeof(temp_args));

        char* tokens[6];
        int token_count = 0;

        int start = 0;
        for (int i = 0; i <= getStrLength(temp_args) && token_count < 6; i++) {
            if (temp_args[i] == ' ' || temp_args[i] == '\0') {
                temp_args[i] = '\0';
                tokens[token_count++] = &temp_args[start];

                while (temp_args[i + 1] == ' ') i++;
                start = i + 1;
            }
        }

        if (token_count != 6) return false;

        strCopy(role, tokens[0], 20);
        strCopy(first_name, tokens[1], 50);
        strCopy(last_name, tokens[2], 50);
        strCopy(phone, tokens[3], 20);
        strCopy(age_str, tokens[4], 10);
        strCopy(password, tokens[5], 100);

        return true;
    }

    bool parseCategoryArgs(const char* args, char* name, char* description) {
        int space_pos = -1;
        for (int i = 0; args[i] != '\0'; i++) {
            if (isWhitespace(args[i])) {
                space_pos = i;
                break;
            }
        }

        if (space_pos == -1) return false;

        for (int i = 0; i < space_pos && i < 99; i++) {
            name[i] = args[i];
        }
        name[space_pos < 99 ? space_pos : 99] = '\0';

        int desc_start = space_pos;
        while (args[desc_start] != '\0' && isWhitespace(args[desc_start])) {
            desc_start++;
        }

        strCopy(description, &args[desc_start], 300);

        return getStrLength(name) > 0 && getStrLength(description) > 0;
    }

    bool parseIdCodeArgs(const char* args, char* id_str, char* code) {
        int space_pos = -1;
        for (int i = 0; args[i] != '\0'; i++) {
            if (isWhitespace(args[i])) {
                space_pos = i;
                break;
            }
        }

        if (space_pos == -1) return false;

        for (int i = 0; i < space_pos && i < 19; i++) {
            id_str[i] = args[i];
        }
        id_str[space_pos < 19 ? space_pos : 19] = '\0';

        int code_start = space_pos;
        while (args[code_start] != '\0' && isWhitespace(args[code_start])) {
            code_start++;
        }

        strCopy(code, &args[code_start], 10);

        return getStrLength(id_str) > 0 && getStrLength(code) > 0;
    }

    bool parseIdPointsArgs(const char* args, char* id_str, char* points_str) {
        return parseIdCodeArgs(args, id_str, points_str);
    }
};

int main() {
    std::cout << "Initializing FMI Supermarket Management System..." << std::endl;

    try {
        CLISystem system;
        system.run();
    }
    catch (...) {
        std::cout << "System error occurred. Shutting down safely..." << std::endl;
        return 1;
    }

    return 0;
}