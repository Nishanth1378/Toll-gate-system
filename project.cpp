#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <ctime>
#include <cstdio>

using namespace std;

class Vehicle {
public:
    enum Type { CAR, TRUCK, BUS, MOTORCYCLE };

    Vehicle(Type type, const string& plateNumber)
        : type(type), plateNumber(plateNumber) {}

    Type getType() const { return type; }
    const string& getPlateNumber() const { return plateNumber; }
    string getTypeString() const {
        switch (type) {
            case CAR: return "Car";
            case TRUCK: return "Truck";
            case BUS: return "Bus";
            case MOTORCYCLE: return "Motorcycle";
            default: return "Unknown";
        }
    }

private:
    Type type;
    string plateNumber;
};

class TollGate {
public:
    TollGate() {
        tollFees[Vehicle::CAR] = 50;
        tollFees[Vehicle::TRUCK] = 100;
        tollFees[Vehicle::BUS] = 75;
        tollFees[Vehicle::MOTORCYCLE] = 30;
        initializeCSV();
    }

    void processVehicle(const Vehicle& vehicle) {
        int fee = tollFees[vehicle.getType()];
        string timestamp = getCurrentTimestamp();
        cout << "Vehicle " << vehicle.getPlateNumber()
                  << " is a " << vehicle.getTypeString()
                  << ". Toll fee: Rs. " << fee << endl;
        
        totalRevenue += fee;
        vehicleCount++;
        logToCSV(vehicle.getPlateNumber(), vehicle.getTypeString(), fee, timestamp);
    }

    void displayStatistics() {
        cout << "\nTotal vehicles processed: " << vehicleCount
             << "\nTotal revenue collected: Rs. " << totalRevenue << "\n";
    }

    void displayAllRecords() {
        ifstream file("toll_records.csv");
        if (!file.is_open()) {
            cerr << "Error: Unable to open toll records file!" << endl;
            return;
        }
        
        string line;
        cout << "\nToll Records:\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    }

    void searchVehicle(const string& plateNumber) {
        ifstream file("toll_records.csv");
        if (!file.is_open()) {
            cerr << "Error: Unable to open toll records file!" << endl;
            return;
        }
        
        string line, plate, type, fee, timestamp;
        bool found = false;
        
        cout << "\nSearch Results for " << plateNumber << ":\n";
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, plate, ',');
            getline(ss, type, ',');
            getline(ss, fee, ',');
            getline(ss, timestamp, ',');
            
            if (plate == plateNumber) {
                cout << "Plate: " << plate << ", Type: " << type << ", Fee: Rs. " << fee << ", Timestamp: " << timestamp << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No records found for vehicle " << plateNumber << endl;
        }
        file.close();
    }

private:
    map<Vehicle::Type, int> tollFees;
    int totalRevenue = 0;
    int vehicleCount = 0;

    void initializeCSV() {
        ofstream file("toll_records.csv", ios::app);
        if (file.tellp() == 0) {
            file << "PlateNumber,VehicleType,Fee,Timestamp\n";
        }
        file.close();
    }

    void logToCSV(const string& plate, const string& type, int fee, const string& timestamp) {
        ofstream file("toll_records.csv", ios::app);
        if (!file.is_open()) {
            cerr << "Error: Unable to write to toll records file!" << endl;
            return;
        }
        file << plate << "," << type << "," << fee << "," << timestamp << "\n";
        file.close();
    }

    string getCurrentTimestamp() {
        time_t now = time(0);
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buffer);
    }
};

int main() {
    TollGate tollGate;
    
    tollGate.processVehicle(Vehicle(Vehicle::CAR, "KA01AB1234"));
    tollGate.processVehicle(Vehicle(Vehicle::TRUCK, "KA05CD5678"));
    tollGate.processVehicle(Vehicle(Vehicle::BUS, "KA03EF9012"));
    tollGate.processVehicle(Vehicle(Vehicle::MOTORCYCLE, "KA07GH3456"));

    tollGate.displayStatistics();
    tollGate.displayAllRecords();
    
    tollGate.searchVehicle("KA01AB1234");
    
    return 0;
}
