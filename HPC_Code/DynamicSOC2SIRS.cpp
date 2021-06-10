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

int L;
int L2;

///////////////////////////////
class network
{
public:
    ////      first three bit from right are evolve stat and first three bit from left are last stat
    ////        as example  A:1000,  B:0100, AB:1100, a:0010, b:0001, AB:1100, Ab:1001, aB;0110, ab;0011, S:0000
    vector < bitset<8> > all_state; ///  list of previous state and evolve state for each indivisuals
    vector<int> Neighbor;

    list<int> A_list1;
    list<int> B_list1;

    list<int> A_list2;
    list<int> B_list2;

    list<int> a_list;
    list<int> b_list;

    set<int> s_list;

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
    net.s_list = {};

    return net;
}

////////////////////////////////


////A:1000,  B:0100, AB:1100, a:0010, b:0001, Ab:1001, aB;0110, ab;0011, S:0000

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
                        net.s_list.erase(*neigh);
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
                        net.s_list.erase(*neigh);
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
                        net.s_list.erase(*neigh);
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
                        net.s_list.erase(*neigh);
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


    for (list<int>::iterator imm=net.a_list.begin();imm != net.a_list.end();++imm){
        float q_random = uniform_dis(gen);

        if (q_random < proper_P) {
            imm_count++;
            net.all_state[*imm].flip(1);
            net.all_state[*imm].flip(5);
            if (!net.all_state[*imm][6]) {
                net.s_list.insert(*imm);
            }
            imm = net.a_list.erase(imm);
        }
    }

    for (list<int>::iterator imm= net.b_list.begin();imm !=  net.b_list.end();++imm){

        float q_random = uniform_dis(gen);
        if (q_random < proper_P) {
            imm_count++;
            net.all_state[*imm].flip(0);
            net.all_state[*imm].flip(4);
            if (!net.all_state[*imm][7]) {
                net.s_list.insert(*imm);
            }
            imm = net.b_list.erase(imm);
        }
    }

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
            net.s_list.erase(sus);

            net.light.flip(0);
            net.light.flip(1);

            A_count++;
            B_count++;

        } else if (random > 0.33 && random < 0.66) { //// S to A
            net.all_state[sus].flip(7);
            net.all_state[sus].flip(3);
            net.A_list1.push_back(sus);
            net.s_list.erase(sus);
            net.light.flip(0);
            A_count++;
        } else if (random > 0.66) { //// S to B
            net.all_state[sus].flip(6);
            net.all_state[sus].flip(2);
            net.B_list1.push_back(sus);
            net.s_list.erase(sus);

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
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"
/////////////////////////////////////
int main()
{
    srand(time(nullptr));

//////////////// Define and Initialize all Parameters /////////
    ///
    float p, q, h, r, l, l_prim, rl;                                    ///
    int length = 1024;                                       ///
    L = length;
    string len = "1024";                                     ///
    int lattice_size = length * length;                     ///
    L2 = lattice_size;
    p = 0.50;                                               ///
    q = 0.99;                                               ///
    h = 0.80;                                               ///
    r = 0.3;
    rl = 20000000;
    l_prim = r / rl;                                              ///
    l = l_prim * L2;
    ///
    // l_prim = l / L2;
    // rl = r / l_prim;
    int lightning_time;                                     ///
    int total_fire = 100000;                                ///
    int save_step = 1000;                                     ///
    int Count_A = 0;                                        ///
    int Count_B = 0;                                        ///
    int CA, CB;                                             ///
    int fire_lifetime = 0;                                  ///
    ///
//    int init_S;
    int init_a, init_b;                             ///
    int immune_count;                                       ///

    random_device rd;
    mt19937 gen(rd());
    exponential_distribution<float> exp_dis(l);
    uniform_int_distribution<int> uniform_dis(0,lattice_size);
    ///
    ///
    ///

    vector<vector<int>> count_results;                      ///
    count_results.reserve(save_step);                       ///
    ///
///////////////////////////////////////////////////////////////

    ostringstream filenames(",");
    filenames.precision(2);
    filenames << "res" << length << "_" << p << "_" << rl << ".dat";
    string filename = filenames.str();


    network the_network;
    the_network = create(the_network,lattice_size);

    for (int i = 0; i < lattice_size; i++){
        the_network.s_list.insert(i);
    }

    init_a = the_network.a_list.size();
    init_b = the_network.b_list.size();
//    init_S = the_network.s_list.size();
//    init_S = L2 - (init_b + init_a);

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
        the_network.s_list.erase(sus);   ///
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
        the_network.s_list.erase(sus);///
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
        the_network.s_list.erase(sus);///
        ///
        the_network.light.flip(1);                   ///
        ///
        Count_B++;                                           ///
    }                                                        ///
////////////////////////////////////////////////////////////////

    int n = 0;
    int k = 0;


    while (n <= total_fire){
        if ((CA + CB) == 0){
            n++;
            k++;
            /// save with vector
            count_results.insert(count_results.end(),{{Count_A, Count_B, fire_lifetime, static_cast<int>(the_network.light.to_ulong()), init_a, init_b, immune_count}});
            Count_A = 0;
            Count_B = 0;
            fire_lifetime = 0;
            ///////////////////
            lightning_time = int(exp_dis(gen));

            tie(the_network,CA, CB, immune_count) = immunization(the_network,r,lightning_time);


            Count_A += CA;
            Count_B += CB;

            init_a = the_network.a_list.size();
            init_b = the_network.b_list.size();
//            init_S = the_network.s_list.size();

//            init_S = L2 - (init_b + init_a);

            continue;
        }
        tie(the_network,CA, CB) = transmission(the_network,p,q,h);
        fire_lifetime += 2;
        Count_A += CA;
        Count_B += CB;


        if (k == save_step) {
//            cout << n << endl;
            k = 0;
            ofstream  count_results_file(filename, ios_base::app | ios_base::out);
            ostream_iterator<int> output_iterator1(count_results_file, ",");
            for (auto & count_result : count_results) {
                copy(count_result.begin(), count_result.end(), output_iterator1);
                count_results_file << "\n";
            }
            count_results_file.close();
            count_results.clear();
        }
    }
}
#pragma clang diagnostic pop