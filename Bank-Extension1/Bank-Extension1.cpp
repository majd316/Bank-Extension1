#include <iostream>
#include<string>
#include<fstream>
#include<vector>
#include<iomanip>
#include <cctype>

using namespace std;

const string FileName = "ClientsData.txt";

void ShowMainMenue();

void ShowTransactionsMenue();

enum enMainMenueOption { enClientList = 1, enAddNewClient = 2, enDeleteClient = 3, enUpdateClient = 4, enFindClient = 5, enTransaction = 6, enExit = 7 };

enum enTransactionOption { enDeposit = 1, enWithdraw = 2, enTotalBalances = 3, enMainMenue = 4 };


struct stClient
{
    string AccountNumber;
    string Name;
    string PinCode;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplitString(string Line, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = Line.find(Delim)) != std::string::npos)
    {
        sWord = Line.substr(0, pos);
        if (sWord != "")
        {
            vString.push_back(sWord);
        }
        Line.erase(0, pos + Delim.length());
    }

    if (Line != "")
    {
        vString.push_back(Line);
    }

    return vString;
}

string ConvertRecordToLine(stClient Client, string Seperetor = "#//#")
{
    string stLine = "";
    stLine += Client.AccountNumber + Seperetor;
    stLine += Client.PinCode + Seperetor;
    stLine += Client.Name + Seperetor;
    stLine += Client.Phone + Seperetor;
    stLine += to_string(Client.AccountBalance);

    return stLine;
}

stClient ConvertLineToRecord(string Line, string Seperetor = "#//#")
{
    vector<string> vClient;
    vClient = SplitString(Line, Seperetor);

    stClient Client;

    Client.AccountNumber = vClient[0];
    Client.PinCode = vClient[1];
    Client.Name = vClient[2];
    Client.Phone = vClient[3];
    Client.AccountBalance = stod(vClient[4]);

    return Client;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector<stClient> vClient;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }

            vClient.push_back(Client);
        }

        MyFile.close();

    }

    return false;
}

void AddClientToFile(string FileName, string Line)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << Line << endl;

        MyFile.close();
    }
}

vector<stClient> LoadClientDataFromFile(string FileName)
{
    fstream MyFile;
    vector<stClient> vClient;
    MyFile.open(FileName, ios::in);



    if (MyFile.is_open())
    {
        stClient Client;
        string Line;

        while (getline(MyFile, Line))
        {
            Client = ConvertLineToRecord(Line);
            vClient.push_back(Client);

        }

        MyFile.close();
    }

    return vClient;
}

stClient ReadNewClient()
{
    stClient Client;


    cout << "\n\nAdding New Client :\n";

    cout << "Enter Account Number ? ";
    getline(cin >> ws, Client.AccountNumber);



    while (ClientExistsByAccountNumber(Client.AccountNumber, FileName))
    {
        cout << "\nClient whiht [" << Client.AccountNumber << "] already exists,Enter another account Number ? ";
        getline(cin >> ws, Client.AccountNumber);
    }


    cout << "Enter Pin Code ? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.AccountBalance;



    return Client;
}

void AddNewClient()
{
    stClient Client;
    Client = ReadNewClient();
    AddClientToFile(FileName, ConvertRecordToLine(Client));
}

void AddClients()
{
    char AddMore = 'n';

    do
    {


        AddNewClient();

        cout << "\n\nClient Added successfully, do you want to add more clients ? Y/N ? ";
        cin >> AddMore;

    } while (AddMore == 'y' || AddMore == 'Y');


}

void PrintClientCard(stClient Client)
{
    cout << "\nThe following ara the client details :\n";
    cout << "------------------------------------------------\n";

    cout << "Account Number   : " << Client.AccountNumber;
    cout << "\nPin Code         : " << Client.PinCode;
    cout << "\nName             : " << Client.Name;
    cout << "\nPhone            : " << Client.Phone;
    cout << "\nAccount Balance  : " << Client.AccountBalance;

    cout << "\n------------------------------------------------\n";

}

void PrintClient(stClient Client)
{
    cout << "|" << setw(15) << left << Client.AccountNumber;
    cout << "|" << setw(10) << left << Client.PinCode;
    cout << "|" << setw(40) << left << Client.Name;
    cout << "|" << setw(12) << left << Client.Phone;
    cout << "|" << setw(12) << left << Client.AccountBalance;

}

void ShowAllClientsScreen()
{
    vector<stClient> vClient = LoadClientDataFromFile(FileName);

    cout << "\t\t\t\tClient List (" << vClient.size() << ") Client(s)\n\n";
    cout << "\n_____________________________________________________________";
    cout << "__________________________________________________________\n" << endl;
    cout << "|" << setw(15) << left << "Account Number";
    cout << "|" << setw(10) << left << "Pin Code";
    cout << "|" << setw(40) << left << "Client Name";
    cout << "|" << setw(12) << left << "Phone";
    cout << "|" << setw(12) << left << "Account Balance";
    cout << "\n_________________________________________________________________";
    cout << "______________________________________________________\n" << endl;

    if (vClient.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";

    else

        for (stClient& Client : vClient)
        {
            PrintClient(Client);

            cout << endl;
        }

    cout << "\n_________________________________________________________________";
    cout << "______________________________________________________\n" << endl;

}

bool FindClientByAccountNumber(string AccountNumber, vector<stClient> vClient, stClient& Client)
{
    for (stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;

            return true;
        }
    }
    return false;

}

bool MarkForDeleteByAccountNumber(string AccountNumer, vector<stClient>& vClient)
{
    stClient Client;

    for (stClient& C : vClient)
    {
        if (C.AccountNumber == AccountNumer)
        {
            C.MarkForDelete = true;
            return true;
        }
    }

    return false;
}

vector<stClient> SaveClientDataToFile(string FileName, vector<stClient>& vClient)
{

    fstream MyFile;

    MyFile.open(FileName, ios::out);
    string Line;

    if (MyFile.is_open())
    {
        for (stClient& C : vClient)
        {
            if (C.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(C);
                MyFile << Line << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

string ReadAccountNumber()
{
    string AccountNumber = "";
    cout << "Please Enter Account Number ? ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool DeleteClient(string AccountNumber, vector<stClient>& vClient)
{

    stClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {

        PrintClientCard(Client);



        cout << "\n\n\nAre you sure you want to delete this client ? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            MarkForDeleteByAccountNumber(AccountNumber, vClient);

            SaveClientDataToFile(FileName, vClient);

            vClient = LoadClientDataFromFile(FileName);

            cout << "\n\nClient Deleted Successfully.";

            return true;
        }

    }
    else
    {
        cout << "\n\nClient whith Account Number (" << AccountNumber << ") is Not Found ! \n";

        return false;
    }



}

stClient ChangeClientData(string AccountNumber)
{
    stClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "Enter Pin Code ? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name ? ";
    getline(cin, Client.Name);

    cout << "Enter Phone ? ";
    getline(cin, Client.Phone);

    cout << "Enter Account Balance ? ";
    cin >> Client.AccountBalance;



    return Client;
}

bool UpdateClientData(string AccountNumber, vector<stClient>& vClient)
{

    stClient Client;

    char Answer = 'n';


    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);



        cout << "\n\n\nAre you sure you want to update this client ? y/n ? ";
        cin >> Answer;

        if (Answer == 'Y' || Answer == 'y')
        {
            for (stClient& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientData(AccountNumber);
                    break;
                }

            }


            SaveClientDataToFile(FileName, vClient);

            cout << "\n\nClient Updated Successfully.";

            return true;
        }

    }
    else
    {
        cout << "\n\nClient whith Account Number (" << AccountNumber << ") is Not Found ! \n";

        return false;
    }



}

bool DepositByAccountNumber(string AccountNumber, double Amount, vector<stClient>& vClient)
{
    char Answer = 'n';


    cout << "\n\nAre you sure you want to perform this transaction ? Y/N ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        for (stClient& C : vClient)
        {
            C.AccountBalance += Amount;
            SaveClientDataToFile(FileName, vClient);

            cout << "\n\nDone Successfully, New balance is : " << C.AccountBalance;

            return true;
        }

    }


    return false;

}


void TotalBalances(stClient Client)
{
    cout << "|" << setw(15) << left << Client.AccountNumber;
    cout << "|" << setw(40) << left << Client.Name;
    cout << "|" << setw(12) << left << Client.AccountBalance;
}

void ShowDepositScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\t\tDeposit Screen";
    cout << "\n------------------------------------------\n";

    stClient Client;

    vector <stClient> vClient = LoadClientDataFromFile(FileName);

    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        cout << "\n\nClient whith Account Number (" << AccountNumber << ") dose Not exist. \n";
        AccountNumber = ReadAccountNumber();
    }
    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease Enter Deposit amount ? ";
    cin >> Amount;

    DepositByAccountNumber(AccountNumber, Amount, vClient);
}

void ShowWithdrawScreen()
{
    cout << "\n------------------------------------------\n";
    cout << "\t\tWithdraw Screen";
    cout << "\n------------------------------------------\n";

    stClient Client;
    vector <stClient> vClient = LoadClientDataFromFile(FileName);

    string AccountNumber = ReadAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        cout << "\n\nClient whith Account Number (" << AccountNumber << ") dose Not exist. \n";
        AccountNumber = ReadAccountNumber();
    }

    PrintClientCard(Client);
    double Amount = 0;
    cout << "\nPlease Enter Withdraw amount ? ";
    cin >> Amount;

    while (Amount > Client.AccountBalance)
    {
        cout << "\n\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "Please Enter another amount ? ";
        cin >> Amount;
    }


    DepositByAccountNumber(AccountNumber, Amount * -1, vClient);
}

void ShowTotalBalances()
{
    vector<stClient> vClient = LoadClientDataFromFile(FileName);

    double Total = 0;

    cout << "\t\t\t\tBalances List (" << vClient.size() << ") Client(s)\n\n";
    cout << "\n_____________________________________________________________";
    cout << "__________________________________________________________\n" << endl;
    cout << "|" << setw(15) << left << "Account Number";
    cout << "|" << setw(40) << left << "Client Name";
    cout << "|" << setw(12) << left << "Account Balance";
    cout << "\n_____________________________________________________________";
    cout << "__________________________________________________________\n" << endl;

    if (vClient.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";

    else

        for (stClient& Client : vClient)
        {
            TotalBalances(Client);
            Total += Client.AccountBalance;
            cout << endl;
        }

    cout << "\n_____________________________________________________________";
    cout << "__________________________________________________________\n" << endl;
    cout << "\t\t\tTotal Balances = " << Total;


}

void ShowTotalBalancesScreen()
{
    ShowTotalBalances();
}

void ShowDeleteClientScreen()
{
    cout << "\n\n---------------------------------------\n";
    cout << "\t Delete Client Screen\n";
    cout << "---------------------------------------\n";

    vector<stClient> vClient = LoadClientDataFromFile(FileName);

    string AccountNumber = ReadAccountNumber();
    DeleteClient(AccountNumber, vClient);

}

void ShowUpdateClientScreen()
{

    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";
    vector <stClient> vClient = LoadClientDataFromFile(FileName);

    string AccountNumber = ReadAccountNumber();

    UpdateClientData(AccountNumber, vClient);

}

void ShowAddNewClientScreen()
{
    cout << "\n\n---------------------------------------\n";
    cout << "\t Add New Client Screen\n";
    cout << "---------------------------------------\n";

    AddClients();


}

void ShowFindClientScreen()
{
    cout << "\n\n---------------------------------------\n";
    cout << "\t Find Client Screen\n";
    cout << "---------------------------------------\n";

    vector<stClient> vClient = LoadClientDataFromFile(FileName);
    stClient Client;
    string AccountNumber = ReadAccountNumber();

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);

    }

    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";

}

void ShowEndScreen()
{
    cout << "\n\n---------------------------------------\n";
    cout << "\t\tProgram Ends :-)\n";
    cout << "---------------------------------------\n";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to main Menue...";
    system("pause>0");
    ShowMainMenue();
}

void GoBackToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transactoin Menue...";
    system("pause>0");
    ShowTransactionsMenue();
}

short ReadMainMenueOption()
{
    short Choice = 0;
    cout << "Choose what do you want to do ? [1 to 7] ? ";
    cin >> Choice;

    return Choice;
}

short ReadTransactionsMenueOption()
{
    short Choice = 0;
    cout << "Choose what do you want to do ? [1 to 4] ? ";
    cin >> Choice;

    return Choice;
}

void PerformTransactionMenueOption(enTransactionOption TransactionOption)
{
    switch (TransactionOption)
    {
    case enTransactionOption::enDeposit:
    {
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenue();
        break;
    }

    case enTransactionOption::enWithdraw:
    {
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionMenue();
        break;
    }

    case enTransactionOption::enTotalBalances:
    {
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenue();
        break;
    }

    case enTransactionOption::enMainMenue:
    {
        system("cls");
        ShowMainMenue();
        break;
    }

    }
}

void PerformMainMenueOption(enMainMenueOption MainMenueOption)
{

    switch (MainMenueOption)
    {

    case enMainMenueOption::enClientList:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOption::enAddNewClient:
    {
        system("cls");
        ShowAddNewClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOption::enDeleteClient:
    {
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOption::enUpdateClient:
    {
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOption::enFindClient:
    {

        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;
    }

    case enMainMenueOption::enTransaction:
    {
        system("cls");
        ShowTransactionsMenue();
        break;
    }

    case enMainMenueOption::enExit:
    {
        system("cls");
        ShowEndScreen();

        break;

    }
    }


}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Exit.\n";
    cout << "===========================================\n";

    PerformMainMenueOption(enMainMenueOption(ReadMainMenueOption()));
}

void ShowTransactionsMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tTransaction Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balances.\n";
    cout << "\t[4] Main Menue.\n";

    cout << "===========================================\n";

    PerformTransactionMenueOption(enTransactionOption(ReadTransactionsMenueOption()));
}


int main()
{


    ShowMainMenue();

    system("pause>0");

    return 0;
}


