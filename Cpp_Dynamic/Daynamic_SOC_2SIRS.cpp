//#include<windows.h>

#include <vector>
#include <tuple>
#include <set>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <iterator>
#include <bitset>
#include <algorithm>
#include <list>
#include <sstream>
#include <random>
#include <cmath>

using namespace std;

///////////////////////////////
class network
{
public:
    ////      first three bit from right are evolve stat and first three bit from left are last stat
    ////        as example  A:1000,  B:0100, AB:1100, a:0010, b:0001, AB:1100, Ab:1001, aB;0110, ab;0011, S:0000
    ////        S = 0, a = 1, b = 2, ab = 3, A = 4, B = 5, aB = 6, Ab = 7, AB = 8
    vector < bitset<8> > all_state; ///  list of previous state and evolve state for each individuals

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

    for(int i = 0; i < data.size() - 1; i++)
    {
        net.all_state.emplace_back(bitset<8>(0));
        net.Neighbor.push_back(data[i]);
    }
    net.A_list = {};
    net.a_list = {};
    net.b_list = {};
    net.B_list = {};
    return net;
}

////////////////////////////////


////        A:1000,  B:0100, AB:1100, a:0010, b:0001, Ab:1001, aB;0110, ab;0011, S:0000
////        S = 0, a = 1, b = 2, ab = 3, A = 4, B = 5, aB = 6, Ab = 7, AB = 8


//network dynamics_2SIRS(network net,float p, float q, float h, float r, float f){
//
/// it was merge of Transmission and Immunization Function in last version of code
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

tuple<network, int, int> transmission(network net,float p, float q, float h) {

    int A_count = 0;
    int B_count = 0;

    set<int> infected;
    set_union(net.A_list.begin(), net.A_list.end(),
              net.B_list.begin(), net.B_list.end(), inserter(infected, infected.begin()));

    set<int> got_sick = {};

    for (int inf : infected) { /// be infected

        /// AB
        if (net.all_state[inf][7] && net.all_state[inf][6]) {
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss A or B

                float random = float(rand() % 100) / 100;
                if (random < 0.5) {    /// AB to aB
                    net.all_state[inf].flip(3);
                    net.all_state[inf].flip(1);
                    net.A_list.erase(inf);
                    net.a_list.insert(inf);
                } else if (random > 0.5) {    /// AB to Ab
                    net.all_state[inf].flip(2);
                    net.all_state[inf].flip(0);
                    net.B_list.erase(inf);
                    net.b_list.insert(inf);
                }
            }

            /// transmission
            for (auto neigh : net.Neighbor[inf]) {

                if (got_sick.find(neigh) == got_sick.end()){
                    /// neighbor is S
                    if (net.all_state[neigh].none()) {  /// neighbor is S

                        if (float(rand() % 100) / 100 < p) {
                            got_sick.insert(neigh);
                            float random = float(rand() % 100) / 100;
                            if (random < 0.5) { /// S to A
                                net.all_state[neigh].flip(3);
                                net.A_list.insert(neigh);
                                net.s_list.erase(neigh);
                                A_count++;
                            } else {              /// S to B
                                net.all_state[neigh].flip(2);
                                net.B_list.insert(neigh);
                                net.s_list.erase(neigh);
                                B_count++;
                            }
                        }
                    }

                        /// neighbor has A or a
                    else if (!(net.all_state[neigh][6] || net.all_state[neigh][4] || net.all_state[neigh].none())) { /// neighbor has A or a
                        if (float(rand() % 100) / 100 < q) {
                            got_sick.insert(neigh);
                            net.all_state[neigh].flip(2);
                            net.B_list.insert(neigh);
                            B_count++;
                        }
                    }

                        /// neighbor has B or b
                    else if (!(net.all_state[neigh][7] || net.all_state[neigh][5] || net.all_state[neigh].none())) { /// neighbor has B or b
                        if (float(rand() % 100) / 100 < q) {
                            got_sick.insert(neigh);
                            net.all_state[neigh].flip(3);
                            net.A_list.insert(neigh);
                            A_count++;
                        }
                    }
                }
            }
        }

        /// A or Ab
        else if (net.all_state[inf][7] && !net.all_state[inf][6]) { /// A or Ab
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss A
                                                            /// AB to a or aB or ab
                net.all_state[inf].flip(3);
                net.all_state[inf].flip(1);
                net.A_list.erase(inf);
                net.a_list.insert(inf);
            }
            /// transmission
            for (auto neigh : net.Neighbor[inf]) {

                if (got_sick.find(neigh) == got_sick.end()) {
                    if (!(net.all_state[neigh][7] || net.all_state[neigh][5])) { /// the neighbor is not A or a
                        if (net.all_state[neigh][6] != net.all_state[neigh][4]) { /// Neighbor is B or b
                            if (float(rand() % 100) / 100 < q) {      /// B to AB or Ab
                                got_sick.insert(neigh);
                                net.all_state[neigh].flip(3);
                                net.A_list.insert(neigh);
                                A_count++;
                            }
                        } else if (net.all_state[neigh].none()) {   /// Neighbor is S
                            if (float(rand() % 100) / 100 < p) {       /// S to A
                                got_sick.insert(neigh);
                                net.all_state[neigh].flip(3);
                                net.A_list.insert(neigh);
                                net.s_list.erase(neigh);
                                A_count++;
                            }
                        }
                    }
                }
            }
        }

        /// B or Ba
        else if (net.all_state[inf][6] && !net.all_state[inf][7]) { /// have B
            /// health
            if (float(rand() % 100) / 100 < h) {  /// loss B
                /// B? to b or Ab or ab
                net.all_state[inf].flip(2);
                net.all_state[inf].flip(0);
                net.B_list.erase(inf);
                net.b_list.insert(inf);
            }
            /// transmission
            for (auto neigh : net.Neighbor[inf]) {
                if (got_sick.find(neigh) == got_sick.end()) {
                    if (!(net.all_state[neigh][6] || net.all_state[neigh][4])) {    /// the neighbor is not A or a
                        if (net.all_state[neigh][7] != net.all_state[neigh][5]) { /// Neighbor is A or a
                            if (float(rand() % 100) / 100 < q) {      /// A to AB or aB
                                got_sick.insert(neigh);
                                net.all_state[neigh].flip(2);
                                net.B_list.insert(neigh);
                                B_count++;
                            }
                        } else if (net.all_state[neigh].none()) {                   /// Neighbor is S
                            if (float(rand() % 100) / 100 < p) {       /// S to B
                                got_sick.insert(neigh);
                                net.all_state[neigh].flip(2);
                                net.B_list.insert(neigh);
                                net.s_list.erase(neigh);
                                B_count++;
                            }
                        }
                    }
                }
            }
        }
    }

    for (auto & j : net.all_state){
        j = j << 4;
        j = j | (j >> 4);
    }
    return make_tuple(net, A_count, B_count);
}

tuple<network,int,int,int> immunization(network net,float r, float time){

    int A_count = 0;
    int B_count = 0;
    int imm_count = 0;
    float proper_P = 1 - exp(-r*time);

    set<int> immune;
    set_union(net.a_list.begin(), net.a_list.end(),
              net.b_list.begin(), net.b_list.end(),inserter(immune, immune.begin()));


    ////////////////////////// Immune
        for (int imm : immune) {

            float q_random = float(rand() % 100) / 100;
            if (q_random < proper_P) {
                imm_count++;
                if (net.all_state[imm][5] && net.all_state[imm][4]) { //// be ab
                    float random = float(rand() % 100) / 100;
                    if (random < 0.5) { ///  to b
                        net.all_state[imm].flip(1);
                        net.a_list.erase(imm);
                    } else { /// to a
                        net.all_state[imm].flip(0);
                        net.b_list.erase(imm);
                    }
                } else if (net.all_state[imm][5] && !net.all_state[imm][4]) { /// be a
                    net.all_state[imm].flip(1);
                    net.a_list.erase(imm);
                    if (!net.all_state[imm][6]) {
                        net.s_list.insert(imm);
                    }
                } else if (net.all_state[imm][4] && !net.all_state[imm][5]) { /// be a
                    net.all_state[imm].flip(0);
                    net.b_list.erase(imm);
                    if (!net.all_state[imm][7]) {
                        net.s_list.insert(imm);
                    }
                }
            }
        }

    for (auto & j : net.all_state){
        j = j << 4;
        j = j | (j >> 4);
    }
////////////////////////// lightning

    net.light = 0;
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

            A_count++;
            B_count++;

        } else if (random > 0.33 && random < 0.66) { //// S to A
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.A_list.insert(sus);
            net.s_list.erase(sus);

            net.light.flip(0);
            A_count++;

        } else if (random > 0.66) { //// S to B
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.B_list.insert(sus);
            net.s_list.erase(sus);

            net.light.flip(1);
            B_count++;

        }
    }
    return make_tuple(net, A_count, B_count, imm_count);
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

    while(true)
    {
        int number = 0;
        adjlist.emplace_back();
        getline(graph, line);
        if(line.length() == 0)
            break;

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

    string path = "/home/respeli/All_Programing_Stuff/Self-Organized-Criticality-on-Spreading-of-Cooperative-Diseases/Cpp_Dynamic/";

    cout<<"please enter lattice length and outbreak probability, like 256 , 0.55 "<<"\n";
    cin >> length >> p;

//////////////// Define and Initialize all Parameters /////////
    int lattice_size = length * length;                     ///
//    p = 0.75;                                             ///
    q = 0.99;                                               ///
    h = 0.80;                                               ///
    r = 0.21;                                               ///
    l = 0.1;                                                ///
                                                            ///
//    float mean_lightning_time = 1 / l;                    ///
    int lightning_time;                                     ///
    int total_step = 300000;                                ///
    int save_step = 10000;                                  ///
    int Count_A = 0;                                        ///
    int Count_B = 0;                                        ///
    int CA, CB;                                             ///
    int fire_lifetime = 0;                                  ///
                                                            ///
    int init_S, init_a, init_b;                             ///
    int immune_count;                                       ///
                                                            ///
    random_device rd;                                       ///
    mt19937 gen(rd());                                      ///
    exponential_distribution<float> exp_dis(l);             ///
                                                            ///
                                                            ///
//    vector<vector<int>> in_time_results;                  ///
//    in_time_results.reserve(save_step);                   ///
    vector<vector<int>> count_results;                      ///
    count_results.reserve(save_step);                       ///
                                                            ///
//    clock_t tStart = clock();                             ///
///////////////////////////////////////////////////////////////


    ostringstream filenames(",");
    filenames.precision(2);
    filenames << "res" << length << "," << p << ".txt";
    string filename = filenames.str();


/////// read adjacency matrix from file and make 2d vector for adjacency matrix ///////////////////
    vector<vector<int>> adjlist;                                                                ///
                                                                                                ///
    adjlist = AdjList(path + "AdjList" + to_string(length) +"lattice.txt",lattice_size);   ///
                                                                                                ///
/////// creat network structure and Quantify with adjacency matrix ////////////////////////////////

    network the_network;
    the_network = create(the_network,adjlist);

    for (int i =0; i < adjlist.size() - 1; i++){
        the_network.s_list.insert(i);
    }
    init_S = the_network.s_list.size();
    init_a = the_network.a_list.size();
    init_b = the_network.b_list.size();
    immune_count = 0;

///////// first step is lightning //////////////////////////////
    the_network.light = 0;                                   ///
    int sus = rand() % the_network.s_list.size();            ///
                                                             ///
    float random = float(rand() % 100) / 100;                ///
    if (random < 0.33) {   //// S to AB                      ///
        the_network.all_state[sus].flip(7);          ///
        the_network.all_state[sus].flip(3);          ///
        the_network.all_state[sus].flip(6);          ///
        the_network.all_state[sus].flip(2);          ///
        the_network.A_list.insert(sus);                      ///
        the_network.B_list.insert(sus);                      ///
        the_network.s_list.erase(sus);                       ///
                                                             ///
        the_network.light.flip(0);                   ///
        the_network.light.flip(1);                   ///
                                                             ///
        Count_A++;                                           ///
        Count_B++;                                           ///
                                                             ///
    }                                                        ///
    else if (random > 0.33 && random < 0.66) { //// S to A   ///
        the_network.all_state[sus].flip(7);          ///
        the_network.all_state[sus].flip(3);          ///
        the_network.A_list.insert(sus);                      ///
        the_network.s_list.erase(sus);                       ///
                                                             ///
        the_network.light.flip(0);                   ///
                                                             ///
        Count_A++;                                           ///
    }                                                        ///
    else if (random > 0.66) { //// S to B                    ///
        the_network.all_state[sus].flip(6);          ///
        the_network.all_state[sus].flip(2);          ///
        the_network.B_list.insert(sus);                      ///
        the_network.s_list.erase(sus);                       ///
                                                             ///
        the_network.light.flip(1);                   ///
                                                             ///
        Count_B++;                                           ///
    }                                                        ///
////////////////////////////////////////////////////////////////


    for(int k = 1 ; k <= total_step ; k++){

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         save with vector, in time result                                                                                                           //
//        in_time_results.insert(in_time_results.end(),{{static_cast<int>(the_network.A_list.size()), static_cast<int>(the_network.a_list.size()),    //
//                                              static_cast<int>(the_network.B_list.size()), static_cast<int>(the_network.b_list.size()),             //
//                                              static_cast<int>(the_network.light.to_ulong())}});                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (the_network.A_list.empty() && the_network.B_list.empty()){
            /// save with vector
            count_results.insert(count_results.end(),{{ Count_A,  Count_B, fire_lifetime, static_cast<int>(the_network.light.to_ulong()), init_S, init_a, init_b, immune_count}});
            Count_A = 0;
            Count_B = 0;
            fire_lifetime = 0;
            ///////////////////
            lightning_time = int(exp_dis(gen));
            tie(the_network,CA, CB, immune_count) = immunization(the_network,r,lightning_time);
            Count_A += CA;
            Count_B += CB;

            init_S = the_network.s_list.size();
            init_a = the_network.a_list.size();
            init_b = the_network.b_list.size();

            continue;
        }

        tie(the_network,CA, CB) = transmission(the_network,p,q,h);
        fire_lifetime ++;
        Count_A += CA;
        Count_B += CB;


        if (k % save_step == 0) {

//            printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);

            ofstream  count_results_file(path + filename, ios_base::app | ios_base::out);
            ostream_iterator<int> output_iterator1(count_results_file, ",");
            for (auto res: count_results) {
                copy(res.begin(), res.end(), output_iterator1);
                count_results_file << "\n";
            }
            count_results_file.close();
            count_results.clear();

/////////////////////////////////////////////////////////////////////////////////////////////////
//            ofstream in_time_results_file(path + filename, ios_base::app | ios_base::out);   //
//            ostream_iterator<int> output_iterator2(in_time_results_file, ",");               //
//            for (auto res: in_time_results) {                                                //
//                copy(res.begin(), res.end(), output_iterator2);                              //
//                in_time_results_file << "\n";                                                //
//            }                                                                                //
//            in_time_results_file.close();                                                    //
//            in_time_results.clear();                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////
//            tStart = clock();
        }
    }
}
#pragma clang diagnostic pop
