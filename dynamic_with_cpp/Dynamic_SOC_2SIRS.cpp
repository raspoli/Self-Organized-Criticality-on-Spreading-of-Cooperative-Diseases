//#include<windows.h>

#include<vector>
#include<set>
#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<string>
#include<iterator>
#include<bitset>
#include<algorithm>
#include<list>
#include<sstream>
#include <random>
#include <cmath>

using namespace std;

///////////////////////////////
class network
{
public:
    ////      first three bit from right are evolve stat and first three bit from left are last stat
    ////        as example  A:1000,  B:0100, AB:1100, a:0010, b:0001, AB:1100, Ab:1001, aB;0110, ab;0011, S:0000
    vector < bitset<8> > all_state; ///  list of previous state and evolve state for each indivisuals
                        ////      0000,0000
    vector<vector<int> > Neighbor;

    set<int> A_list;
    set<int> B_list;
    set<int> a_list;
    set<int> b_list;
    set<int> s_list;

    bitset<2> light;

};

//////////////////////////////////////

network create(network net, vector<vector<int> > data)
{
    net.all_state.reserve(data.size());
    net.Neighbor.reserve(data.size());

//    bitset<8> is;
//    is = 0;

    for(int i = 0; i < data.size() - 1; i++)
    {
        net.all_state.emplace_back(bitset<8>(0));
        net.Neighbor.push_back(data[i]);
//        cout << net.all_state[i] << "\n";
    }
    net.A_list = {};
    net.a_list = {};
    net.b_list = {};
    net.B_list = {};
    return net;
}

////////////////////////////////


////A:1000,  B:0100, AB:1100, a:0010, b:0001, Ab:1001, aB;0110, ab;0011, S:0000


//network dynamics_2SIRS(network net,float p, float q, float h, float r, float f){
//
/// it was merge of Transmition and Immunation Function in last version of code
//
// }
//

//////////////////////////

//////// Dynamics of 2SIRS ////////////
#pragma clang diagnostic push
#pragma ide diagnostic ignored "modernize-use-auto"
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "bugprone-branch-clone"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"

network transmission(network net,float p, float q, float h) {

    set<int> infected;
    set_union(net.A_list.begin(), net.A_list.end(),
              net.B_list.begin(), net.B_list.end(), inserter(infected, infected.begin()));

    for (int inf : infected) { /// be infected

        /// AB
        if (net.all_state[inf][7] && net.all_state[inf][6]) {  /// AB
//            cout << "AB is here,\n";
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss A or B

                float random = float(float(rand() % 100) / 100);
                if (random < 0.5) {    /// AB? to aB
                    net.all_state[inf].flip(3);
                    net.all_state[inf].flip(1);
                    net.A_list.erase(inf);
                    net.a_list.insert(inf);
//                    cout << "A- a+,";
                } else if (random > 0.5) {    /// AB to Ab
                    net.all_state[inf].flip(2);
                    net.all_state[inf].flip(0);
                    net.B_list.erase(inf);
                    net.b_list.insert(inf);
//                    cout << "B- b+,";
                }
            }

            /// transmission
            for (auto neigh : net.Neighbor[inf]) {

                /// neighbor is S
                if (net.all_state[neigh].none()) {  /// neighbor is S

                    if (float(rand() % 100) / 100 < p) {

                        float random = float(rand() % 100) / 100;

                        if (random < 0.5) { /// S to A
                            net.all_state[neigh].flip(3);
                            net.A_list.insert(neigh);
                            net.s_list.erase(neigh);
                            //                                cout << "A+,";
                        } else if (random > 0.5) { /// S to B
                            net.all_state[neigh].flip(2);
                            net.B_list.insert(neigh);
                            net.s_list.erase(neigh);
//                                cout << "B+,";
                        }
                    }
                }

                    /// neighbor has A or a
                else if (!(net.all_state[neigh][6] || net.all_state[neigh][4])) { /// neighbor has A or a
                    if (float(rand() % 100) / 100 < q) {
                        net.all_state[neigh].flip(2);
                        net.B_list.insert(neigh);
//                            cout << "B+,";
                    }
                }

                    /// neighbor has B or b
                else if ((net.all_state[neigh][7] || net.all_state[neigh][5])) { /// neighbor has B or b
                    if (float(rand() % 100) / 100 < q) {
                        net.all_state[neigh].flip(3);
                        net.A_list.insert(neigh);
//                            cout << "A+,";
                    }
                }
            }
        }

            /// A or A
        else if (net.all_state[inf][7] && !net.all_state[inf][6]) { /// A or Ab
//            cout << "A is here,\n";
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss A
                /// A? to a or aB or ab
                net.all_state[inf].flip(3);
                net.all_state[inf].flip(1);
                net.A_list.erase(inf);
                net.a_list.insert(inf);
//                cout << "A- a+,";
            }

            /// transmission
            for (auto neigh : net.Neighbor[inf]) {

                if (!(net.all_state[neigh][7] || net.all_state[neigh][5])) { /// the neighbor is not A or a

                    if (net.all_state[neigh][6] != net.all_state[neigh][4]) { /// Neighbor is B or b
                        if (float(rand() % 100) / 100 < q) {      /// B to AB or Ab
                            net.all_state[neigh].flip(3);
                            net.A_list.insert(neigh);
//                            cout << "A+,";
                        }
                    } else if (net.all_state[neigh].none()) {   /// Neighbor is S
                        if (float(rand() % 100) / 100 < p) {       /// S to A
                            net.all_state[neigh].flip(3);
                            net.A_list.insert(neigh);
                            net.s_list.erase(neigh);
//                            cout << "A+,";
                        }
                    }
                }
            }
        }

            /// B or Ba
        else if (net.all_state[inf][6] && !net.all_state[inf][7]) { /// have B
//            cout << "B is here,\n";
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss B
                /// B? to b or Ab or ab
                net.all_state[inf].flip(2);
                net.all_state[inf].flip(0);
                net.B_list.erase(inf);
                net.b_list.insert(inf);
//                cout << "B- b+,";
            }
            /// transmission
            for (auto neigh : net.Neighbor[inf]) {

                if (!(net.all_state[neigh][6] || net.all_state[neigh][4])) {    /// the neighbor is not A or a

                    if (net.all_state[neigh][7] != net.all_state[neigh][5]) { /// Neighbor is A or a
                        if (float(rand() % 100) / 100 < q) {      /// A to AB or aB
                            net.all_state[neigh].flip(2);
                            net.B_list.insert(neigh);
//                            cout << "B+,";
                        }
                    } else if (net.all_state[neigh].none()) {                   /// Neighboor is S
                        if (float(rand() % 100) / 100 < p) {       /// S to B
                            net.all_state[neigh].flip(2);
                            net.B_list.insert(neigh);
                            net.s_list.erase(neigh);
//                            cout << "B+,";
                        }
                    }
                }
            }
        }
        else{
            net.A_list.erase(inf);
            net.B_list.erase(inf);

            if (net.all_state[inf][0]){
                net.a_list.insert(inf);
            }
            if (net.all_state[inf][1]){
                net.b_list.insert(inf);
            }
        }
    }

    for (auto & j : net.all_state){
        j = j << 4;
        j = j | (j >> 4);
    }
    net.light = 0;
    return net;
}

network immunization(network net,float r, float time){

    set<int> immune;
    set_union(net.a_list.begin(), net.a_list.end(),
              net.b_list.begin(), net.b_list.end(),inserter(immune, immune.begin()));

    ////////////////////////// Immune
        for (int imm : immune) {

            float q_random = float(rand() % 100) / 100;
            if (q_random < r && q_random > (r * pow((1 - r), time))) {
                if (net.all_state[imm][5] && net.all_state[imm][4]) { //// be ab
//                    if (float(rand() % 100) / 100 < r) {
                    float random = float(rand() % 100) / 100;
                    if (random < 0.5) { ///  to b
                        net.all_state[imm].flip(1);
                        net.a_list.erase(imm);
                        //                        cout << "a-,";
                    } else if (random > 0.5) { /// to a
                        net.all_state[imm].flip(0);
                        net.b_list.erase(imm);
                        //                        cout << "b-,";
                    }
//                    }
                } else if (net.all_state[imm][5] && !net.all_state[imm][4]) { /// be a
//                    if (float(rand() % 100) / 100 < r) { //// to S or B
                    net.all_state[imm].flip(1);
                    net.a_list.erase(imm);
                    //                    cout << "a-,";
                    if (!net.all_state[imm][6]) {
                        net.s_list.insert(imm);
                    }
//                    }
                } else if (net.all_state[imm][4] && !net.all_state[imm][5]) { /// be a
//                    if (float(rand() % 100) / 100 < r) {
                    net.all_state[imm].flip(0);
                    net.b_list.erase(imm);
                    //                    cout << "b-,";
                    if (!net.all_state[imm][7]) {
                        net.s_list.insert(imm);
                        }
//                    }
                }
            }
        }

    for (auto & j : net.all_state){
        j = j << 4;
        j = j | (j >> 4);
    }
////////////////////////// lightning
    if (!net.s_list.empty()) {
        int sus = rand() % net.s_list.size();

        float random = float(rand() % 100) / 100;
        if (random < 0.33) {   //// S to AB
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.A_list.insert(sus);
            net.B_list.insert(sus);
            net.s_list.erase(sus);

            net.light.flip(0);
            net.light.flip(1);
        } else if (random > 0.33 && random < 0.66) { //// S to A
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.A_list.insert(sus);
            net.s_list.erase(sus);

            net.light.flip(0);
        } else if (random > 0.66) { //// S to B
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.B_list.insert(sus);
            net.s_list.erase(sus);

            net.light.flip(1);
        }
    }
    return net;
}
#pragma clang diagnostic pop


/////////////////////////////////////
vector<vector<int>> AdjList(const string& path, int size){

    ifstream graph;
    graph.open(path);
    string line;

    vector<vector<int>> adjlist;
    adjlist.reserve(size);

    int x = 0;
//    int o = 0;


    while(true)
    {
        int number = 0;
        adjlist.emplace_back();
        getline(graph, line);
        if(line.length() == 0)
            break;
//        o = 0;

        for(int i = 1; i < line.length(); i++)
        {
            ////   just for lattice
            adjlist[x].reserve(4);

            if(line[i] >= '0' && line[i] <= '9')
            {
                number = number * 10 + int(line[i]-'0');
            }
            else if(line[i] == ',' || line[i] == ']')
            {
                adjlist[x].push_back(number);
                number = 0;
//                o++;
            }
            if(line[i] == ']')
                break;
        }
        x++;
    }
    graph.close();

    return adjlist;
}

///// Function for read and write, Network Data as state and lists
//vector < bitset<8> > read_stat(const string& path, int size){
//    ifstream state;
//    state.open(path);
//    string line;
//
//    vector < bitset<8> > states;
//    states.reserve(size);
//
//    int o = 3;
//    bitset<8> number = 0;
//
//    getline(state, line);
//
//    for(int i = 1; i < line.length(); i++)
//    {
//        if(line[i] == '1')
//        {
//            number.flip(o);
//        }
//        else if(line[i] == ',' || line[i] == ']')
//        {
//            states.emplace_back(number);
//            number = 0;
//            o = 3;
//        }
//        if(line[i] == 'e')
//            break;
//        o--;
//    }
//
//    state.close();
//
//    return states;
//}


//vector <set<int>> read_list(const string& path, int size){
//    ifstream list;
//    list.open(path);
//    string line;
//    cout << 5;
//    vector<set <int>> q_list;
//
//    int x = 0;
////    , o = 3;
//    while (true) {
//        int number = 0;
//        getline(list, line);
//
//        if(line.length() == 0){
//            cout << 0 << "\n";
//            break;}
//
//        for (int i = 1; i < line.length(); i++) {
//            q_list[x] = {};
//            if (line[i] >= '0' && line[i] <= '9') {
//                number = number * 10 + int(line[i] - '0');
//            }
//            else if (line[i] == ',') {
//                cout << 5;
//                q_list[x].insert(number);
//                number = 0;
//            }
//            if (line[i] == 'e')
//                break;
//        }
//        x++;
//        cout << 1 << "\n";
//    }
//    list.close();
//
//    return q_list;
//}

//void save_network_details(const network& net,const string& path,const string& filename){
//    ofstream s_file(path+"state"+filename);
//    for (auto sta: net.all_state) {
//        s_file << sta << ",";
//    }
//    s_file << "e";
//
//    ofstream l_file(path+"list"+filename);
//    for (auto Al: net.A_list) {
//        l_file << Al << ",";
//    }
//    l_file << "e" << "\n";
//
//    for (auto al: net.a_list) {
//        l_file << al << ",";
//    }
//    l_file << "e" << "\n";
//
//    for (auto Bl: net.B_list) {
//        l_file << Bl << ",";
//    }
//    l_file << "e" << "\n";
//
//    for (auto bl: net.b_list) {
//        l_file << bl << ",";
//    }
//    l_file << "e" << "\n";
//
//    for (auto sl: net.s_list) {
//        l_file << sl << ",";
//    }
//    l_file << "e" << "\n";
//}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "cert-msc51-cpp"
/////////////////////////////////////
int main()
{

    srand(time(nullptr));

    float p, q, h, r, l;
    int length;

    string path = "/home/respeli/All_Programing_Stuff/Self-Organized-Criticality-on-Spreading-of-Cooperative-Diseases/dynamic_with_cpp/";

    cout<<"please enter lattice length and outbreak probability, like 256 , 0.55 "<<"\n";
    cin >> length >> p;


    int lattice_size = length * length;

//    p = 0.75;
    q = 0.99;
    h = 0.80;
    r = 0.21;
    l = 0.003;


    ostringstream filenames(",");
    filenames.precision(2);
    filenames << length << "," << p << ".txt";
    string filename = filenames.str();



    // read adjacency matrix from file and make 2d vector for adjacency matrix
    string line;
    vector<vector<int>> adjlist;

    adjlist = AdjList(path + "AdjList" + to_string(length) +"lattice.txt",lattice_size);

    // creat network structure and Quantify with adjacency matrix

    network the_network;
    the_network = create(the_network,adjlist);

    for (int i =0; i < adjlist.size() - 1; i++){
        the_network.s_list.insert(i);
    }

//     first step is lightning

    int sus = rand() % the_network.s_list.size();

    float random = float(rand() % 100) / 100;
    if (random < 0.33) {   //// S to AB
        the_network.all_state[sus].flip(7);
        the_network.all_state[sus].flip(3);
        the_network.all_state[sus].flip(6);
        the_network.all_state[sus].flip(2);
        the_network.A_list.insert(sus);
        the_network.B_list.insert(sus);
        the_network.s_list.erase(sus);

        the_network.light.flip(0);
        the_network.light.flip(1);
    }
    else if (random > 0.33 && random < 0.66) { //// S to A
        the_network.all_state[sus].flip(7);
        the_network.all_state[sus].flip(3);
        the_network.A_list.insert(sus);
        the_network.s_list.erase(sus);

        the_network.light.flip(0);
    }
    else if (random > 0.66) { //// S to B
        the_network.all_state[sus].flip(6);
        the_network.all_state[sus].flip(2);
        the_network.B_list.insert(sus);
        the_network.s_list.erase(sus);

        the_network.light.flip(1);
    }



    random_device rd;
    mt19937 gen(rd());

    float mean_lightning_time = 1 / l;

    normal_distribution<float> norm_dis(mean_lightning_time,sqrt(mean_lightning_time));
    int lightning_time;

    vector<vector<int>> results;

    int save_step = 10000;
    results.reserve(save_step);

    clock_t tStart = clock();

    for(int k = 1 ; k <= 300000 ; k++){

        cout << k << "\n";
        /// save with vector
        results.insert(results.end(),{{static_cast<int>(the_network.A_list.size()), static_cast<int>(the_network.a_list.size()),
                                              static_cast<int>(the_network.B_list.size()), static_cast<int>(the_network.b_list.size()),
                                              static_cast<int>(the_network.light.to_ulong())}});
        if (the_network.A_list.empty() && the_network.B_list.empty()){
            lightning_time = int(norm_dis(gen));
            the_network = immunization(the_network,r,lightning_time);
            continue;
        }
        the_network = transmission(the_network,p,q,h);


        if (k % save_step == 0) {

            printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);

            ofstream file(path + filename, ios_base::app | ios_base::out);
            ostream_iterator<int> output_iterator(file, ",");
            for (auto res: results) {
                copy(res.begin(), res.end(), output_iterator);
                file << "\n";
            }
            file.close();
            results.clear();
            tStart = clock();
        }
    }
}
#pragma clang diagnostic pop
