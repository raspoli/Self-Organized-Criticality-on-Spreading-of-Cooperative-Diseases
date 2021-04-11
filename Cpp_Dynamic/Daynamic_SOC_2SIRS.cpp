//#include<windows.h>
//#include <unordered_map>
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

int L;
int L2;

///////////////////////////////
class network
{
public:
    ////      first three bit from right are evolve stat and first three bit from left are last stat
    ////        as example  A:1000,  B:0100, AB:1100, a:0010, b:0001, AB:1100, Ab:1001, aB;0110, ab;0011, S:0000
    vector < bitset<8> > all_state; ///  list of previous state and evolve state for each indivisuals
//    vector<vector<int> > Neighbor;

    vector<int> Neighbor;

    list<int> A_list1;
    list<int> B_list1;

    list<int> A_list2;
    list<int> B_list2;

    list<int> a_list;
    list<int> b_list;

//    set<int> s_list;

    bitset<2> light;

};

//////////////////////////////////////

network create(network net, int size)
{
    net.all_state.reserve(size);

    net.Neighbor.reserve(4);


    for(int i = 0; i < size; i++)
    {
        net.all_state.emplace_back(bitset<8>(0));
    }
//    net.A_list1 = {};
//    net.B_list1 = {};
//
//    net.A_list2 = {};
//    net.B_list2 = {};
//
//    net.a_list.reserve(size);
//    net.b_list.reserve(size);
//    net.a_list = {};
//    net.b_list = {};

//    net.s_list = {};


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
list<int> got_sick;

tuple<network, int, int> transmission(network net,float p, float q, float h) {

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> uniform_dis(0.0, 1.0);

    int A_count = 0;
    int B_count = 0;
    got_sick.clear();

    for (list<int>::iterator inf = net.A_list1.begin();inf != net.A_list1.end();++inf){

        net.Neighbor = {(*inf+1)%L2, (*inf - 1 + L2)%L2, (*inf + L)%L2, (*inf - L + L2)%L2};

        for (vector<int>::iterator neigh = net.Neighbor.begin(); neigh != net.Neighbor.end();++neigh) {
            if (!net.all_state[*neigh][3]) {
                if (net.all_state[*neigh].none()) {   /// Neighbor is S
                    if (uniform_dis(gen) < p) {       /// S to A
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(3);
                        net.A_list2.insert(net.A_list2.end(),*neigh); /// 2
//                        net.s_list.erase(*neigh);
                        A_count++;
                    }
                }
                else if (!(net.all_state[*neigh][7] || net.all_state[*neigh][5])) { /// the neighbor is not A or a and it is not S
                    if (uniform_dis(gen) < q) {      /// B or b to AB or Ab
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(3);
                        net.A_list2.insert(net.A_list2.end(),*neigh); /// 2
                        A_count++;
                    }
                }
            }
        }
        /// Ab or A to a or ab
        net.all_state[*inf].flip(3);
        net.all_state[*inf].flip(1);
        net.all_state[*inf] = net.all_state[*inf] << 4;
        net.all_state[*inf] = net.all_state[*inf] | (net.all_state[*inf] >> 4);
        net.a_list.insert(net.a_list.end(),*inf);
    }
    net.A_list1.clear();

    for (list<int>::iterator inf = net.B_list1.begin();inf != net.B_list1.end();++inf){

        net.Neighbor = {(*inf+1)%L2, (*inf - 1 + L2)%L2, (*inf + L)%L2, (*inf - L + L2)%L2};

        for (vector<int>::iterator neigh = net.Neighbor.begin(); neigh != net.Neighbor.end();++neigh) {
            if (!net.all_state[*neigh][2]) {
                if (net.all_state[*neigh].none()) {   /// Neighbor is S
                    if (uniform_dis(gen) < p) {       /// S to A
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(2);
                        net.B_list2.insert(net.B_list2.end(),*neigh); /// 2
//                        net.s_list.erase(*neigh);
                        B_count++;
                    }
                }
                else if (!(net.all_state[*neigh][6] || net.all_state[*neigh][4])) { /// the neighbor is not B or b and it is not S
                    if (uniform_dis(gen) < q) {      /// B or b to AB or Ab
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(2);
                        net.B_list2.insert(net.B_list2.end(),*neigh); /// 2
                        B_count++;
                    }
                }
            }
        }
        /// aB or B to ab or b

        net.all_state[*inf].flip(2);
        net.all_state[*inf].flip(0);
        net.all_state[*inf] = net.all_state[*inf] << 4;
        net.all_state[*inf] = net.all_state[*inf] | (net.all_state[*inf] >> 4);
        net.b_list.insert(net.b_list.end(),*inf);

    }
    net.B_list1.clear();

    for (auto j : got_sick){
        net.all_state[j] = net.all_state[j] << 4;
        net.all_state[j] = net.all_state[j] | (net.all_state[j] >> 4);
    }

//////////////////////////////////////////////////////////////////////////////////////////////
    got_sick.clear();

    for (list<int>::iterator inf=net.A_list2.begin();inf!=net.A_list2.end();++inf){

        net.Neighbor = {(*inf+1)%L2, (*inf - 1 + L2)%L2, (*inf + L)%L2, (*inf - L + L2)%L2};

        for (vector<int>::iterator neigh = net.Neighbor.begin(); neigh != net.Neighbor.end();++neigh) {
            if (!net.all_state[*neigh][3]) {
                if (net.all_state[*neigh].none()) {   /// Neighbor is S
                    if (uniform_dis(gen) < p) {       /// S to A
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(3);
                        net.A_list1.insert(net.A_list1.end(),*neigh); /// 2
//                        net.s_list.erase(*neigh);
                        A_count++;
                    }
                }
                else if (!(net.all_state[*neigh][7] || net.all_state[*neigh][5])) { /// the neighbor is not A or a and it is not S
                    if (uniform_dis(gen) < q) {      /// B or b to AB or Ab
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(3);
                        net.A_list1.insert(net.A_list1.end(),*neigh); /// 2
                        A_count++;
                    }
                }
            }
        }
        /// Ab or A to a or ab
        net.all_state[*inf].flip(3);
        net.all_state[*inf].flip(1);
        net.all_state[*inf] = net.all_state[*inf] << 4;
        net.all_state[*inf] = net.all_state[*inf] | (net.all_state[*inf] >> 4);
        net.a_list.insert(net.a_list.end(),*inf);
    }
    net.A_list2.clear();

    for (list<int>::iterator inf=net.B_list2.begin();inf!=net.B_list2.end();++inf){

        net.Neighbor = {(*inf+1)%L2, (*inf - 1 + L2)%L2, (*inf + L)%L2, (*inf - L + L2)%L2};

        for (vector<int>::iterator neigh = net.Neighbor.begin(); neigh != net.Neighbor.end();++neigh) {
            if (!net.all_state[*neigh][2]) {
                if (net.all_state[*neigh].none()) {   /// Neighbor is S
                    if (uniform_dis(gen) < p) {       /// S to A
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(2);
                        net.B_list1.insert(net.B_list1.end(),*neigh); /// 1
//                        net.s_list.erase(*neigh);
                        B_count++;
                    }
                }
                else if (!(net.all_state[*neigh][6] || net.all_state[*neigh][4])) { /// the neighbor is not B or b and it is not S
                    if (uniform_dis(gen) < q) {      /// B or b to AB or Ab
                        got_sick.insert(got_sick.end(),*neigh);
                        net.all_state[*neigh].flip(2);
                        net.B_list1.insert(net.B_list1.end(),*neigh); /// 1
                        B_count++;
                    }
                }
            }
        }
        /// aB or B to ab or b
        net.all_state[*inf].flip(2);
        net.all_state[*inf].flip(0);
        net.all_state[*inf] = net.all_state[*inf] << 4;
        net.all_state[*inf] = net.all_state[*inf] | (net.all_state[*inf] >> 4);
        net.b_list.insert(net.b_list.end(),*inf);
    }
    net.B_list2.clear();

    for (auto j : got_sick){
        net.all_state[j] = net.all_state[j] << 4;
        net.all_state[j] = net.all_state[j] | (net.all_state[j] >> 4);
    }

    return make_tuple(net, A_count, B_count);
}

tuple<network,int,int,int> immunization(network net,float r, float time){

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> uniform_dis(0.0, 1.0);
    uniform_int_distribution<int> uniform_int(0, L2);

    int A_count = 0;
    int B_count = 0;
    int imm_count = 0;
    float proper_P = 1 - exp(-r*time);

    list<int> who_changed;

    for (list<int>::iterator imm=net.a_list.begin();imm != net.a_list.end();++imm){
        float q_random = uniform_dis(gen);

        if (q_random < proper_P) {
            imm_count++;
            net.all_state[*imm].flip(1);
            net.all_state[*imm].flip(5);

//            who_changed.insert(who_changed.end(),*imm);
//            net.all_state[*imm] = net.all_state[*imm] << 4;
//            net.all_state[*imm] = net.all_state[*imm] | (net.all_state[*imm] >> 4);

            imm = net.a_list.erase(imm);
//            if (!net.all_state[*imm][6]) {
//                net.s_list.insert(*imm);
//            }
        }
    }

    for (list<int>::iterator imm= net.b_list.begin();imm !=  net.b_list.end();++imm){

        float q_random = uniform_dis(gen);
        if (q_random < proper_P) {
            imm_count++;
            net.all_state[*imm].flip(0);
            net.all_state[*imm].flip(4);

//            who_changed.insert(who_changed.end(),*imm);
            imm = net.b_list.erase(imm);
//            if (!net.all_state[*imm][7]) {
//                net.s_list.insert(*imm);
//            }
//            net.all_state[*imm] = net.all_state[*imm] << 4;
//            net.all_state[*imm] = net.all_state[*imm] | (net.all_state[*imm] >> 4);
        }
    }

//    for (auto & j : who_changed){
//        net.all_state[j] = net.all_state[j] << 4;
//        net.all_state[j] = net.all_state[j] | (net.all_state[j] >> 4);
//    }
////////////////////////// lightning

    net.light = 0;
    int sus = uniform_int(gen);
    while (net.all_state[sus][0] && net.all_state[sus][1]){
        sus = uniform_int(gen);
    }

    if (net.all_state[sus].none()) {
        float random = uniform_dis(gen);
        if (random < 0.33) {   //// S to AB
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.A_list1.push_back(sus);
            net.B_list1.push_back(sus);

            net.light.flip(0);
            net.light.flip(1);

            A_count++;
            B_count++;

        } else if (random > 0.33 && random < 0.66) { //// S to A
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.A_list1.push_back(sus);
            //        net.s_list.erase(sus);

            net.light.flip(0);
            A_count++;
        } else if (random > 0.66) { //// S to B
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.B_list1.push_back(sus);
            //        net.s_list.erase(sus);

            net.light.flip(1);
            B_count++;
        }
    } else if (net.all_state[sus][1]){
        net.all_state[sus].flip(6);
        net.all_state[sus].flip(2);
        net.B_list1.push_back(sus);
        net.light.flip(1);
        B_count++;
    } else if (net.all_state[sus][0]){
        net.all_state[sus].flip(7);
        net.all_state[sus].flip(3);
        net.A_list1.push_back(sus);
        net.light.flip(0);
        A_count++;
    }

    return make_tuple(net, A_count, B_count, imm_count);
}
#pragma clang diagnostic pop


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

//    string path = "/home/respeli/All_Programing_Stuff/SOC_on_hpc/";


//////////////// Define and Initialize all Parameters /////////
    ///
    float p, q, h, r, l;                                    ///
    int length = 64;                                       ///
    L = length;
    string len = "64";                                     ///
    int lattice_size = length * length;                     ///
    L2 = lattice_size;
    p = 0.65;                                               ///
    q = 0.99;                                               ///
    h = 0.80;                                               ///
    r = 0.21;                                               ///
    l = 0.1;                                                ///
    ///
//    float mean_lightning_time = 1 / l;                    ///
    int lightning_time;                                     ///
    int total_fire = 1000;                                ///
    int save_step = 100;                                     ///
    int Count_A = 0;                                        ///
    int Count_B = 0;                                        ///
    int CA, CB;                                             ///
    int fire_lifetime = 0;                                  ///
    ///
    int init_S, init_a, init_b;                             ///
    int immune_count;                                       ///

    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<float> exp_dis(l);
    uniform_int_distribution<int> uniform_dis(0,lattice_size);
                                                            ///
                                                            ///
                                                            ///
//    vector<vector<int>> in_time_results;                  ///
//    in_time_results.reserve(save_step);                   ///
    vector<vector<int>> count_results;                      ///
    count_results.reserve(save_step);                       ///
    ///
    clock_t tStart = clock();                             ///
///////////////////////////////////////////////////////////////

//    cout<<"please enter lattice length and outbreak probability, like 256 , 0.55 "<<"\n";
//    cin >> length >> p;

    ostringstream filenames(",");
    filenames.precision(2);
    filenames << "res" << length << "_" << p << ".dat";
    string filename = filenames.str();


/////// read adjacency matrix from file and make 2d vector for adjacency matrix ///////////////////
//    vector<vector<int>> adjlist;                                                                ///
    ///
//    adjlist = AdjList("AdjList" + len +"lattice.txt",lattice_size);                         ///
    ///
/////// creat network structure and Quantify with adjacency matrix ////////////////////////////////

    network the_network;
    the_network = create(the_network,lattice_size);

//    for (int i = 0; i < lattice_size; i++){
//        the_network.s_list.insert(i);
//    }

    init_a = the_network.a_list.size();
    init_b = the_network.b_list.size();
    init_S = L2 - (init_b + init_a);

    immune_count = 0;

///////// first step is lightning //////////////////////////////
    ///
    int sus = uniform_dis(gen);            ///
                                                             ///
    float random = float(rand() % 100) / 100;                ///
    if (random < 0.33) {   //// S to AB                      ///
        the_network.all_state[sus].flip(7);          ///
        the_network.all_state[sus].flip(3);          ///
        the_network.all_state[sus].flip(6);          ///
        the_network.all_state[sus].flip(2);          ///
        the_network.A_list1.push_back(sus);                      ///
        the_network.B_list1.push_back(sus);                      ///
//        the_network.s_list.erase(sus);                       ///
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
        ///
        the_network.A_list1.push_back(sus);                      ///
        ///
//        the_network.s_list.erase(sus);                       ///
        ///
        the_network.light.flip(0);                   ///
        ///
        Count_A++;                                           ///
    }                                                        ///
    else if (random > 0.66) { //// S to B                    ///
        the_network.all_state[sus].flip(6);          ///
        the_network.all_state[sus].flip(2);          ///
        ///
        the_network.B_list1.push_back(sus);                      ///
        ///
//        the_network.s_list.erase(sus);                       ///
        ///
        the_network.light.flip(1);                   ///
        ///
        Count_B++;                                           ///
    }                                                        ///
////////////////////////////////////////////////////////////////
//    cout << the_network.B_list1.size() << endl;
//    cout << the_network.A_list1.size() << endl;

    int n = 0;
    int k = 0;

    clock_t tStart_t = clock();
    clock_t tStart_i;
    while (n <= total_fire){
//        cout << k << endl;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//         save with vector, in time result                                                                                                           //
//        in_time_results.insert(in_time_results.end(),{{static_cast<int>(the_network.A_list.size()), static_cast<int>(the_network.a_list.size()),    //
//                                              static_cast<int>(the_network.B_list.size()), static_cast<int>(the_network.b_list.size()),             //
//                                              static_cast<int>(the_network.light.to_ulong())}});                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        if (the_network.A_list1.empty() && the_network.B_list1.empty()){
//        if ((CA + CB) == 0){
//            printf("t Time: %.2fs\n", (double) (clock() - tStart_t) / CLOCKS_PER_SEC);
            n++;
            k++;
            /// save with vector
            count_results.insert(count_results.end(),{{Count_A, Count_B, fire_lifetime, static_cast<int>(the_network.light.to_ulong()), init_S, init_a, init_b, immune_count}});
            Count_A = 0;
            Count_B = 0;
            fire_lifetime = 0;
            ///////////////////
            lightning_time = int(exp_dis(gen));

//            tStart_i = clock();
            tie(the_network,CA, CB, immune_count) = immunization(the_network,r,lightning_time);
//            printf("i Time: %.2fs\n", (double) (clock() - tStart_i) / CLOCKS_PER_SEC);


            Count_A += CA;
            Count_B += CB;

//            init_S = the_network.s_list.size();
            init_a = the_network.a_list.size();
            init_b = the_network.b_list.size();
            init_S = L2 - (init_b + init_a);

//            tStart_t = clock();
            continue;
        }
        tie(the_network,CA, CB) = transmission(the_network,p,q,h);
        fire_lifetime += 2;
        Count_A += CA;
        Count_B += CB;


        if (k == save_step) {
            cout << n << endl;
            k = 0;
            ofstream  count_results_file(filename, ios_base::app | ios_base::out);
            ostream_iterator<int> output_iterator1(count_results_file, ",");
            for (auto & count_result : count_results) {
                copy(count_result.begin(), count_result.end(), output_iterator1);
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
//            printf("Time taken: %.2fs\n", (double) (clock() - tStart_t) / CLOCKS_PER_SEC);
//            tStart_t = clock();
        }
    }
    printf("Time taken: %.2f\n", (double) ((clock() - tStart) / CLOCKS_PER_SEC)/60);
}
#pragma clang diagnostic pop
