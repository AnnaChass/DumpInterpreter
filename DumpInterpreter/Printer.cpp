#include "Printer.h"
using namespace std;

int Printer::Print(const s_data& data, const unsigned int n)
{
    if (data.isCorrupted) cout << "CORRUPTED PACKET" << endl;
    cout << "Length: " << dec << data.length << endl <<
        "Protocol discriminator: " << "0x" << hex << data.descriptor << endl;
    
    
    for (int i = 0; i < n; i++)
    {
        cout << "Message " << i + 1 << endl <<
            '\t' << "Tag: " << dec << parameter[data.message[i].tag].descriptor << endl <<
            '\t' << "Length: " << dec << data.message[i].length << endl <<
            '\t' << "Value: ";

        // message contents binary code, so print in hex
        if (parameter[data.message[i].tag].format == binary)
        {
            cout << "0x";
            for (int j = 0; j < data.message[i].length; j++)
            {
                //if (!data.message[i].value[j]) cout << "00"; // std::hex prints 0x00 incorrect, so check this value by hand
                //else 
                cout << setw(2) << setfill('0') << hex << (int)data.message[i].value[j];
            }
        }

        // message contents ASCII code
        else
        {
            for (int j = 0; j < data.message[i].length; j++)
                cout << dec << data.message[i].value[j];
        }

        cout << endl;
    }
    cout << endl << "--------------------------------------------------------------------" << endl << endl;

    return 0;
}
