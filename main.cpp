    #include <iostream>       // For standard input/output operations
    #include <vector>         // For using the vector container
    #include <string>         // For string operations
    #include <sqlite3.h>      // For SQLite database functionality
    #include <iomanip>        // For output formatting (like setw)
    using namespace std;
    const string DB_FILE = "database.db"; // Defines the SQLite database filename
    // Forward declarations
    struct User;              // Forward declaration of User struct
    struct Flight;            // Forward declaration of Flight struct

    struct User {
        string name;          // Stores passenger's name
        string userID;        // Unique identifier for the user
        string flightNumber;  // Flight the user is booked on
        int seatNumber;       // Seat assignment

        void display() const {  // Method to display user information
            cout << left << setw(15) << "Name:" << name << endl;
            cout << setw(15) << "User ID:" << userID << endl;
            cout << setw(15) << "Flight Number:" << flightNumber << endl;
            cout << setw(15) << "Seat Number:" << seatNumber << endl;
        }
    };
    struct Flight {
        string flightNumber;     // Unique flight identifier
        string airlineName;      // Name of the airline
        string startingPoint;    // Departure location
        string destination;      // Arrival location
        int totalTickets;        // Total seats available
        int availableTickets;    // Seats remaining

        void display() const {   // Method to display flight information
            cout << left << setw(20) << "Flight Number:" << flightNumber << endl;
            cout << setw(20) << "Airline Name:" << airlineName << endl;
            cout << setw(20) << "Starting Point:" << startingPoint << endl;
            cout << setw(20) << "Destination:" << destination << endl;
            cout << setw(20) << "Total Tickets:" << totalTickets << endl;
            cout << setw(20) << "Available Tickets:" << availableTickets << endl;
        }
    };

    // Database functions - Interface for all database operations in the system

    // Initializes the database by creating required tables if they don't exist
    // Creates both Flights and Users tables with proper schema constraints
    void initializeDatabase();

    // Executes a SQL command that doesn't return results (INSERT/UPDATE/DELETE/CREATE)
    // @param sql: The SQL command string to execute
    // @return: true if execution succeeded, false on error
    bool executeSQL(const string& sql);

    // Callback function that processes each row of flight query results
    // Executes a SQL query that returns data and processes results with a callback function
    // @param sql: The SQL query string to execute
    // @param callback: Function pointer to handle each row of results
    // @param data: Optional pointer to pass additional data to callback
    // @return: true if execution succeeded, false on error
    bool executeSQLWithCallback(const string& sql, int (*callback)(void*, int, char**, char**), void* data = nullptr);

    // Checks if a flight exists in the database
    // @param flightNumber: Unique identifier for the flight
    // @return: true if flight exists, false otherwise
    bool flightExists(const string& flightNumber);

    // Verifies if a user is registered in the system
    // @param userID: Unique identifier for the user
    // @return: true if user exists, false otherwise
    bool userExists(const string& userID);

    // Determines if a specific seat is available on a given flight
    // @param flightNumber: The flight to check
    // @param seatNumber: The seat number to verify
    // @return: true if seat is available, false if already taken
    bool isSeatAvailable(const string& flightNumber, int seatNumber);

    // Callback function that processes each row of flight query results
    //Processes each row of flight data from database queries.
    // @param data: User-provided pointer for additional context
    // @param argc: Number of columns in the result row
    // @param argv: Array of column values as strings
    // @param azColName: Array of column names
    // @return: 0 to continue processing, non-zero to abort
    int flightCallback(void* data, int argc, char** argv, char** azColName);

    // Callback function that processes each row of user query results
    //Processes each row of user data from database queries.
    // @param data: User-provided pointer for additional context
    // @param argc: Number of columns in the result row
    // @param argv: Array of column values as strings
    // @param azColName: Array of column names
    // @return: 0 to continue processing, non-zero to abort
    int userCallback(void* data, int argc, char** argv, char** azColName);

    // Retrieves all occupied seat numbers for a specific flight
    // @param flightNumber: The flight to check for taken seats
    // @return: Vector containing all occupied seat numbers
    vector<int> getTakenSeats(const string& flightNumber);

    // Management functions - Core operations for the airline reservation system

    // Adds a new flight to the system
    // Prompts user for flight details (number, airline, route, capacity)
    // Validates input and ensures flight doesn't already exist
    void addFlight();

    // Modifies an existing flight's information
    // Prompts for flight number and new details
    // Updates airline, route, or ticket capacity while preserving bookings
    void modifyFlight();

    // Removes a flight from the system
    // Handles cascading deletion of all associated reservations
    // Prompts for confirmation before deletion
    void deleteFlight();

    // Registers a new passenger in the system
    // Collects user details (name, ID) and assigns unique identifier
    // Validates user doesn't already exist
    void addUser();

    // Updates an existing passenger's information
    // Allows changing name or other details while preserving bookings
    // Maintains referential integrity with reservations
    void modifyUser();

    // Removes a passenger from the system
    // Cleans up all associated reservations
    // Updates flight availability counts accordingly
    void deleteUser();

    // Creates a new flight reservation
    // Links user to flight with seat assignment
    // Handles seat availability checks and updates ticket counts
    void makeReservation();

    // Cancels an existing reservation
    // Removes user-flight association
    // Frees up the seat and updates flight availability
    void cancelReservation();

    // Displays all available flights
    // Shows complete flight information in formatted table
    // Includes availability status for each flight
    void displayFlights();

    // Lists all registered passengers
    // Shows user details with their current bookings
    // Formatted for easy readability
    void displayUsers();

    int main() {
        initializeDatabase();
        
        int choice;
        do {
            cout << "\n--- Airline Reservation System ---\n";
            cout << "1. Flight Management\n";
            cout << "2. User Management\n";
            cout << "3. Make Reservation\n";
            cout << "4. Cancel Reservation\n";
            cout << "5. Display Flights\n";
            cout << "6. Display Users\n";
            cout << "7. Show Available Seats\n";
            cout << "0. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore(); // Clear newline character

            switch (choice) {
                case 1: {
                    int flightChoice;
                    cout << "\nFlight Management:\n";
                    cout << "1. Add Flight\n2. Modify Flight\n3. Delete Flight\n";
                    cout << "Enter choice: ";
                    cin >> flightChoice;
                    cin.ignore();
                    
                    if (flightChoice == 1) addFlight();
                    else if (flightChoice == 2) modifyFlight();
                    else if (flightChoice == 3) deleteFlight();
                    else cout << "Invalid choice.\n";
                    break;
                }
                case 2: {
                    int userChoice;
                    cout << "\nUser Management:\n";
                    cout << "1. Add User\n2. Modify User\n3. Delete User\n";
                    cout << "Enter choice: ";
                    cin >> userChoice;
                    cin.ignore();
                    
                    if (userChoice == 1) addUser();
                    else if (userChoice == 2) modifyUser();
                    else if (userChoice == 3) deleteUser();
                    else cout << "Invalid choice.\n";
                    break;
                }
                case 3:
                    makeReservation();
                    break;
                case 4:
                    cancelReservation();
                    break;
                case 5:
                    displayFlights();
                    break;
                case 6:
                    displayUsers();
                    break;
                case 7: {
                    string flightNumber;
                    cout << "Enter Flight Number to see available seats: ";
                    getline(cin, flightNumber);
                    vector<int> takenSeats = getTakenSeats(flightNumber);
                    cout << "Taken seats: ";
                    for (int seat : takenSeats) {
                        cout << seat << " ";
                    }
                    cout << endl;
                    break;
                }
                case 0:
                    cout << "Exiting the system.\n";
                    break;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 0);

        return 0;
    }

    void initializeDatabase() {
        const char* sql = 
            "CREATE TABLE IF NOT EXISTS Flights ("  // Creates Flights table if it doesn't exist
            "flightNumber TEXT PRIMARY KEY,"        // Unique identifier for flights
            "airlineName TEXT NOT NULL,"            // Airline name (required)
            "startingPoint TEXT NOT NULL,"          // Departure city (required)
            "destination TEXT NOT NULL,"            // Arrival city (required)
            "totalTickets INTEGER NOT NULL,"        // Total seats available
            "availableTickets INTEGER NOT NULL);"   // Seats remaining
            
            "CREATE TABLE IF NOT EXISTS Users ("    // Creates Users table if it doesn't exist
            "userID TEXT PRIMARY KEY,"             // Unique passenger ID
            "name TEXT NOT NULL,"                  // Passenger name (required)
            "flightNumber TEXT NOT NULL,"          // Associated flight
            "seatNumber INTEGER NOT NULL,"         // Assigned seat
            "UNIQUE(flightNumber, seatNumber),"    // Ensures no duplicate seats per flight
            "FOREIGN KEY(flightNumber) REFERENCES Flights(flightNumber));";  // Links to Flights table

        if (executeSQL(sql)) {
            cout << "Database initialized successfully\n";
        }
    }

    bool executeSQL(const string& sql) {
        sqlite3* db;                     // Database connection object
        char* errMsg = nullptr;          // For storing error messages
        bool success = false;            // Return status

        if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {  // Open database
            // Execute SQL command
            if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
                /*
                db: Database connection

                sql.c_str(): The SQL command to execute (converted to C-style string)

                nullptr: No callback function (since we're not processing row results)

                nullptr: No data to pass to callback

                &errMsg: Address to store any error message
                
                */
                cerr << "SQL error: " << errMsg << endl;  // Print error if any
                sqlite3_free(errMsg);     // Free error message memory
            } else {
                success = true;           // Mark as successful
            }
            sqlite3_close(db);           // Close database connection
        } else {
            cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        }
        return success;
    }

// Function to execute SQL query with a callback function
bool executeSQLWithCallback(const string& sql, int (*callback)(void*, int, char**, char**), void* data) {
    sqlite3* db;  // Database handle
    char* errMsg = nullptr;  // Error message pointer
    bool success = false;  // Success flag

    // Open database connection
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        // Execute SQL query with callback
        if (sqlite3_exec(db, sql.c_str(), callback, data, &errMsg) != SQLITE_OK) {
            cerr << "SQL error: " << errMsg << endl;  // Print error if query fails
            sqlite3_free(errMsg);  // Free error message memory
        } else {
            success = true;  // Set success flag if query succeeds
        }
        sqlite3_close(db);  // Close database connection
    } else {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;  // Print error if can't open database
    }

    return success;  // Return success status
}

// Check if a flight exists in the database
bool flightExists(const string& flightNumber) {
    sqlite3* db;  // Database handle
    sqlite3_stmt* stmt;  // Prepared statement object
    bool exists = false;  // Existence flag

    // Open database connection
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        const char* sql = "SELECT 1 FROM Flights WHERE flightNumber = ?;";  // SQL query
        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        /*
        ? is a placeholder for the flight number

        sqlite3_prepare_v2 compiles the SQL statement

        -1 means automatic SQL string length detection

        stmt stores the prepared statement
        */
            sqlite3_bind_text(stmt, 1, flightNumber.c_str(), -1, SQLITE_STATIC);  // Bind parameter
            if (sqlite3_step(stmt) == SQLITE_ROW) {  // Execute query
                exists = true;  // Set flag if flight exists
            }
            sqlite3_finalize(stmt);  // Finalize statement
        }
        sqlite3_close(db);  // Close database connection
    }

    return exists;  // Return existence status
}

// Check if a user exists in the database
bool userExists(const string& userID) {
    sqlite3* db;  // Database handle
    sqlite3_stmt* stmt;  // Prepared statement
    bool exists = false;  // Existence flag

    // Open database connection
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        const char* sql = "SELECT 1 FROM Users WHERE userID = ?;";  // SQL query
        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);  // Bind parameter
            if (sqlite3_step(stmt) == SQLITE_ROW) {  // Execute query
                exists = true;  // Set flag if user exists
            }
            sqlite3_finalize(stmt);  // Finalize statement
        }
        sqlite3_close(db);  // Close database connection
    }

    return exists;  // Return existence status
}

// Check if a seat is available on a flight
bool isSeatAvailable(const string& flightNumber, int seatNumber) {
    sqlite3* db;  // Database handle
    sqlite3_stmt* stmt;  // Prepared statement
    bool available = true;  // Availability flag

    // Open database connection
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        const char* sql = "SELECT 1 FROM Users WHERE flightNumber = ? AND seatNumber = ?;";  // SQL query
        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, flightNumber.c_str(), -1, SQLITE_STATIC);  // Bind first parameter
            sqlite3_bind_int(stmt, 2, seatNumber);  // Bind second parameter
            if (sqlite3_step(stmt) == SQLITE_ROW) {  // Execute query
                available = false;  // Set flag if seat is taken
            }
            sqlite3_finalize(stmt);  // Finalize statement
        }
        sqlite3_close(db);  // Close database connection
    }

    return available;  // Return availability status
}

// Get list of taken seats for a flight
vector<int> getTakenSeats(const string& flightNumber) {
    sqlite3* db;  // Database handle
    sqlite3_stmt* stmt;  // Prepared statement
    vector<int> takenSeats;  // Vector to store taken seats

    // Open database connection
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        const char* sql = "SELECT seatNumber FROM Users WHERE flightNumber = ?;";  // SQL query
        // Prepare SQL statement
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
            sqlite3_bind_text(stmt, 1, flightNumber.c_str(), -1, SQLITE_STATIC);  // Bind parameter
            while (sqlite3_step(stmt) == SQLITE_ROW) {  // Execute query and process results
                takenSeats.push_back(sqlite3_column_int(stmt, 0));  // Add seat number to vector
            }
            sqlite3_finalize(stmt);  // Finalize statement
        }
        sqlite3_close(db);  // Close database connection
    }

    return takenSeats;  // Return vector of taken seats
}

// Callback function to process flight data from SQL query results
// Parameters:
//   data - User-provided pointer (unused in this function)
//   argc - Number of columns in the result row
//   argv - Array of column values for the current row
//   azColName - Array of column names
int flightCallback(void* data, int argc, char** argv, char** azColName) {
    // Create a temporary Flight object to store the current row's data
    Flight flight;  // Flight object to store data
    
    // Loop through each column in the current result row
    for (int i = 0; i < argc; i++) {
        // Get the name of the current column
        string colName = azColName[i];  // Get column name
        
        // Check column name and store the corresponding value in the Flight object
        if (colName == "flightNumber") 
            // Store flight number (use empty string if NULL)
            flight.flightNumber = argv[i] ? argv[i] : "";
        else if (colName == "airlineName") 
            // Store airline name (use empty string if NULL)
            flight.airlineName = argv[i] ? argv[i] : "";
        else if (colName == "startingPoint") 
            // Store starting point (use empty string if NULL)
            flight.startingPoint = argv[i] ? argv[i] : "";
        else if (colName == "destination") 
            // Store destination (use empty string if NULL)
            flight.destination = argv[i] ? argv[i] : "";
        else if (colName == "totalTickets") 
            // Convert totalTickets from string to integer (default to 0 if NULL)
            flight.totalTickets = argv[i] ? atoi(argv[i]) : 0;
        else if (colName == "availableTickets") 
            // Convert availableTickets from string to integer (default to 0 if NULL)
            flight.availableTickets = argv[i] ? atoi(argv[i]) : 0;
    }
    
    // Display the flight information using the Flight object's display method
    flight.display();  // Display flight information
    
    // Print a separator line for better readability between flight records
    cout << "----------------------------------------\n";  // Separator
    
    // Return 0 to indicate successful processing and continue to next row
    return 0;  // Return success
}

// Callback function for user data
int userCallback(void* data, int argc, char** argv, char** azColName) {
    User user;  // User object to store data
    // Process each column in the result row
    for (int i = 0; i < argc; i++) {
        string colName = azColName[i];  // Get column name
        // Map column to user property
        if (colName == "name") user.name = argv[i] ? argv[i] : "";
        else if (colName == "userID") user.userID = argv[i] ? argv[i] : "";
        else if (colName == "flightNumber") user.flightNumber = argv[i] ? argv[i] : "";
        else if (colName == "seatNumber") user.seatNumber = argv[i] ? atoi(argv[i]) : 0;
    }
    user.display();  // Display user information
    cout << "----------------------------------------\n";  // Separator
    return 0;  // Return success
}

// Add a new flight to the database
void addFlight() {
    Flight flight;  // Flight object to store new data
    cout << "\nEnter Flight Number: ";
    getline(cin, flight.flightNumber);  // Get flight number from user
    
    // Check if flight already exists
    if (flightExists(flight.flightNumber)) {
        cout << "Flight with this number already exists!\n";
        return;
    }

    // Get remaining flight details from user
    cout << "Enter Airline Name: ";
    getline(cin, flight.airlineName);
    cout << "Enter Starting Point: ";
    getline(cin, flight.startingPoint);
    cout << "Enter Destination: ";
    getline(cin, flight.destination);
    cout << "Enter Total Tickets: ";
    cin >> flight.totalTickets;
    flight.availableTickets = flight.totalTickets;  // Set available tickets equal to total
    cin.ignore();  // Clear input buffer

    // Build SQL insert statement
    string sql = "INSERT INTO Flights VALUES ('" + flight.flightNumber + "', '" +
                flight.airlineName + "', '" + flight.startingPoint + "', '" +
                flight.destination + "', " + to_string(flight.totalTickets) + ", " +
                to_string(flight.availableTickets) + ");";

    // Execute SQL and show result
    if (executeSQL(sql)) {
        cout << "Flight added successfully.\n";
    }
}

// Modify an existing flight
void modifyFlight() {
    string flightNumber;  // Flight number to modify
    cout << "\nEnter Flight Number to modify: ";
    getline(cin, flightNumber);

    // Check if flight exists
    if (!flightExists(flightNumber)) {
        cout << "Flight not found!\n";
        return;
    }

    Flight flight;  // Flight object for new data
    flight.flightNumber = flightNumber;
    // Get new flight details from user
    cout << "Enter New Airline Name: ";
    getline(cin, flight.airlineName);
    cout << "Enter New Starting Point: ";
    getline(cin, flight.startingPoint);
    cout << "Enter New Destination: ";
    getline(cin, flight.destination);
    cout << "Enter New Total Tickets: ";
    cin >> flight.totalTickets;
    cout << "Enter New Available Tickets: ";
    cin >> flight.availableTickets;
    cin.ignore(); // Clear input buffer

    // Build SQL update statement
    string sql = "UPDATE Flights SET "
        "airlineName = '" + flight.airlineName + "', "
        "startingPoint = '" + flight.startingPoint + "', "
        "destination = '" + flight.destination + "', "
        "totalTickets = " + to_string(flight.totalTickets) + ", "
        "availableTickets = " + to_string(flight.availableTickets) + " "
        "WHERE flightNumber = '" + flightNumber + "';";
    
    // Execute SQL and show result
    if (executeSQL(sql)) {
        cout << "Flight modified successfully.\n";
    }
}

// Delete a flight from the database
void deleteFlight() {
    string flightNumber;  // Flight number to delete
    cout << "\nEnter Flight Number to delete: ";
    getline(cin, flightNumber);

    // Check if flight exists
    if (!flightExists(flightNumber)) {
        cout << "Flight not found!\n";
        return;
    }

    // First delete all users associated with this flight
    string deleteUsersSql = "DELETE FROM Users WHERE flightNumber = '" + flightNumber + "';";
    string deleteFlightSql = "DELETE FROM Flights WHERE flightNumber = '" + flightNumber + "';";
    
    // Execute both deletions and show result
    if (executeSQL(deleteUsersSql) && executeSQL(deleteFlightSql)) {
        cout << "Flight and associated users deleted successfully.\n";
    }
}

// Add a new user to the database
void addUser() {
    User user;  // User object for new data
    cout << "\nEnter User ID: ";
    getline(cin, user.userID);
    
    // Check if user already exists
    if (userExists(user.userID)) {
        cout << "User with this ID already exists!\n";
        return;
    }

    // Get user details from input
    cout << "Enter Name: ";
    getline(cin, user.name);
    cout << "Enter Flight Number: ";
    getline(cin, user.flightNumber);
    
    // Check if flight exists
    if (!flightExists(user.flightNumber)) {
        cout << "Flight doesn't exist!\n";
        return;
    }

    // Get and display taken seats
    vector<int> takenSeats = getTakenSeats(user.flightNumber);
    cout << "Taken seats: ";
    for (int seat : takenSeats) {
        cout << seat << " ";
    }
    cout << "\nEnter Seat Number: ";
    cin >> user.seatNumber;
    cin.ignore(); // Clear input buffer

    // Check if seat is available
    if (!isSeatAvailable(user.flightNumber, user.seatNumber)) {
        cout << "Seat " << user.seatNumber << " is already taken on this flight!\n";
        return;
    }

    // Build SQL insert statement
    string sql = "INSERT INTO Users VALUES ('" + user.userID + "', '" 
        + user.name + "', '" + user.flightNumber + "', " 
        + to_string(user.seatNumber) + ");";
    
    // Execute SQL and update available tickets
    if (executeSQL(sql)) {
        string updateSql = "UPDATE Flights SET availableTickets = availableTickets - 1 "
            "WHERE flightNumber = '" + user.flightNumber + "';";
        executeSQL(updateSql);
        cout << "User added successfully.\n";
    }
}

// Modify an existing user
void modifyUser() {
    string userID;  // User ID to modify
    cout << "\nEnter User ID to modify: ";
    getline(cin, userID);

    // Check if user exists
    if (!userExists(userID)) {
        cout << "User not found!\n";
        return;
    }

    User user;  // User object for new data
    user.userID = userID;
    // Get new user details from input
    cout << "Enter New Name: ";
    getline(cin, user.name);
    cout << "Enter New Flight Number: ";
    getline(cin, user.flightNumber);
    
    // Check if new flight exists
    if (!flightExists(user.flightNumber)) {
        cout << "Flight doesn't exist!\n";
        return;
    }

    cout << "Enter New Seat Number: ";
    cin >> user.seatNumber;
    cin.ignore(); // Clear input buffer

    // Check if new seat is available (excluding current user's seat)
    sqlite3* db;
    sqlite3_stmt* stmt;
    bool seatAvailable = true;

    // Open database connection - returns SQLITE_OK if successful
if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
    
    // SQL query to check if seat is taken by someone else
    // ? marks are parameter placeholders for:
    // 1. flightNumber, 2. seatNumber, 3. current userID (to exclude)
    const char* sql = "SELECT 1 FROM Users WHERE flightNumber = ? AND seatNumber = ? AND userID != ?;";
    
    // Prepare the SQL statement for execution
    // - db: database connection
    // - sql: query string
    // - -1: automatic string length detection
    // - &stmt: will store the prepared statement
    // - nullptr: unused portion of SQL string
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        
        // Bind flightNumber parameter (1st ? placeholder)
        // - stmt: prepared statement
        // - 1: parameter index (1-based)
        // - user.flightNumber.c_str(): value to bind (as C string)
        // - -1: string length (automatic)
        // - SQLITE_STATIC: string won't change during execution
        sqlite3_bind_text(stmt, 1, user.flightNumber.c_str(), -1, SQLITE_STATIC);
        
        // Bind seatNumber parameter (2nd ? placeholder)
        // - 2: parameter index
        // - user.seatNumber: integer value to bind
        sqlite3_bind_int(stmt, 2, user.seatNumber);
        
        // Bind userID parameter (3rd ? placeholder)
        // - 3: parameter index
        // - userID.c_str(): current user's ID to exclude from check
        sqlite3_bind_text(stmt, 3, userID.c_str(), -1, SQLITE_STATIC);
        
        // Execute the prepared statement
        // SQLITE_ROW means a matching record was found (seat is taken)
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            seatAvailable = false;  // Mark seat as unavailable
        }
        
        // Clean up the prepared statement
        sqlite3_finalize(stmt);
    }
    
    // Close the database connection
    sqlite3_close(db);
}

    if (!seatAvailable) {
        cout << "Seat " << user.seatNumber << " is already taken on this flight!\n";
        return;
    }

    // Build SQL update statement
    string sql = "UPDATE Users SET "
        "name = '" + user.name + "', "
        "flightNumber = '" + user.flightNumber + "', "
        "seatNumber = " + to_string(user.seatNumber) + " "
        "WHERE userID = '" + userID + "';";
    
    // Execute SQL and show result
    if (executeSQL(sql)) {
        cout << "User modified successfully.\n";
    }
}

// Delete a user from the database
void deleteUser() {
    string userID;  // User ID to delete
    cout << "\nEnter User ID to delete: ";
    getline(cin, userID);

    // Check if user exists
    if (!userExists(userID)) {
        cout << "User not found!\n";
        return;
    }

    // Get flight number before deleting to update available tickets
    string flightNumber;
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        string sql = "SELECT flightNumber FROM Users WHERE userID = '" + userID + "';";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                flightNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

    // Build and execute delete statement
    string deleteSql = "DELETE FROM Users WHERE userID = '" + userID + "';";
    if (executeSQL(deleteSql)) {
        // Increment available tickets if flight number was found
        if (!flightNumber.empty()) {
            string updateSql = "UPDATE Flights SET availableTickets = availableTickets + 1 "
                "WHERE flightNumber = '" + flightNumber + "';";
            executeSQL(updateSql);
        }
        cout << "User deleted successfully.\n";
    }
}

// Make a flight reservation
void makeReservation() {
    string userID, flightNumber;
    cout << "\nEnter User ID: ";
    getline(cin, userID);
    
    // Check if user already exists
    if (userExists(userID)) {
        cout << "User with this ID already exists!\n";
        return;
    }

    cout << "Enter Flight Number: ";
    getline(cin, flightNumber);

    // Check available tickets
    sqlite3* db;
    sqlite3_stmt* stmt;
    int availableTickets = -1;
    
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        string sql = "SELECT availableTickets FROM Flights WHERE flightNumber = '" + flightNumber + "';";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                availableTickets = sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

    if (availableTickets == -1) {
        cout << "Flight not found.\n";
        return;
    }

    if (availableTickets <= 0) {
        cout << "No available tickets for this flight.\n";
        return;
    }

    // Show taken seats
    vector<int> takenSeats = getTakenSeats(flightNumber);
    cout << "Taken seats: ";
    for (int seat : takenSeats) {
        cout << seat << " ";
    }
    cout << endl;

    User user;
    user.userID = userID;
    cout << "Enter Name: ";
    getline(cin, user.name);
    user.flightNumber = flightNumber;
    cout << "Enter Seat Number: ";
    cin >> user.seatNumber;
    cin.ignore(); // Clear input buffer

    // Check if seat is available
    if (!isSeatAvailable(flightNumber, user.seatNumber)) {
        cout << "Seat " << user.seatNumber << " is already taken on this flight!\n";
        return;
    }

    // Build and execute insert statement
    string insertSql = "INSERT INTO Users VALUES ('" + user.userID + "', '" 
        + user.name + "', '" + user.flightNumber + "', " 
        + to_string(user.seatNumber) + ");";
    
    // Execute SQL and update available tickets
    if (executeSQL(insertSql)) {
        string updateSql = "UPDATE Flights SET availableTickets = availableTickets - 1 "
            "WHERE flightNumber = '" + flightNumber + "';";
        executeSQL(updateSql);
        cout << "Reservation successful! Seat booked.\n";
    }
}

// Cancel a reservation
void cancelReservation() {
    string userID;
    cout << "\nEnter User ID to cancel reservation: ";
    getline(cin, userID);

    // Check if user exists
    if (!userExists(userID)) {
        cout << "User not found!\n";
        return;
    }

    // Get flight number before deleting
    string flightNumber;
    sqlite3* db;
    sqlite3_stmt* stmt;
    
    if (sqlite3_open(DB_FILE.c_str(), &db) == SQLITE_OK) {
        string sql = "SELECT flightNumber FROM Users WHERE userID = '" + userID + "';";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                flightNumber = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            }
            sqlite3_finalize(stmt);
        }
        sqlite3_close(db);
    }

    // Build and execute delete statement
    string deleteSql = "DELETE FROM Users WHERE userID = '" + userID + "';";
    if (executeSQL(deleteSql)) {
        // Update available tickets if flight number was found
        if (!flightNumber.empty()) {
            string updateSql = "UPDATE Flights SET availableTickets = availableTickets + 1 "
                "WHERE flightNumber = '" + flightNumber + "';";
            executeSQL(updateSql);
        }
        cout << "Reservation canceled successfully.\n";
    }
}

// Display all flights
void displayFlights() {
    cout << "\n--- Flight Information ---\n";
    string sql = "SELECT * FROM Flights ORDER BY flightNumber;";
    executeSQLWithCallback(sql, flightCallback);
}

// Display all users
void displayUsers() {
    cout << "\n--- User Information ---\n";
    string sql = "SELECT * FROM Users ORDER BY userID;";
    executeSQLWithCallback(sql, userCallback);
}