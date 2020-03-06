/*-------------------------------------------------------------
 * Module contenant des fonctions utilitaires
 * -----------------------------------------------------------*/
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

using namespace std;

class Utils
{
public:
    Utils();

    static int randInt(int low, int high);

    template<typename T>
    static void permut(T& x, T& y){
        T tmpX;
        tmpX = x;
        T tmpY;
        tmpY = y;
        x = tmpY;
        y = tmpX;
    }

    template<typename T>
    static bool is_in_vector(vector<T> vec, T val)
    {
        for (auto v : vec)
        {
            if (val == v) return true;
        }
        return false;
    }

    template<typename T>
    static vector<T> invert_vector(vector<T> vec)
    {
        if (vec.empty())    return vec;
        vector<T> myVec;
        int cpt=vec.size()-1;
        do
        {
            myVec.push_back(vec[cpt]);
            cpt--;

        } while(cpt>=0);
        return myVec;
    }

    template<typename T>
    static int get_pos_of_val_in_vector(vector<T> vec, T val)
    {
        for (unsigned i=0; i<vec.size(); i++) {
            if (val == vec[i])  return i;
        }
        return -1;
    }

    /*--------------------------------------------------------------
     * Vrai si @val2 est à la suite de @val1 dans le
     * tableau @vec .
     * Est circulaire. (ex: vec[0] peut être suivant de vec[fin])
     * -----------------------------------------------------------*/
    template<typename T>
    static bool is_next_of(vector<T> vec, T val1, T val2)
    {
        if (!Utils::is_in_vector(vec, val1)
                || !Utils::is_in_vector(vec, val2)) {
            cout << "in " << __FUNCTION__ << ": not in vector";
            return false;
        }
        unsigned id1;
        for (unsigned i=0; i<vec.size(); i++)
        {
            if (vec[i] == val1) {
                id1 = i;
            }
        }
        // cout << "in " << __FUNCTION__ << ": id1 = " << id1 << endl;
        if (id1 == vec.size()-1   &&  vec[0] == val2) return true;
        if (id1 == vec.size()-1)    return false;
        if (vec[id1+1] == val2) return true;

        return false;
    }

    /*--------------------------------------------------------------
     * Vrai si @val2 est précédent à @val1 dans le
     * tableau @vec .
     * Est circulaire. (ex: vec[0] peut être suivant de vec[fin])
     * -----------------------------------------------------------*/
    template<typename T>
    static bool is_pred_of(vector<T> vec, T val1, T val2)
    {
        if (!Utils::is_in_vector(vec, val1)
                || !Utils::is_in_vector(vec, val2)) {
            cout << "in " << __FUNCTION__ << ": not in vector";
            return false;
        }
        unsigned id1;
        for (unsigned i=0; i<vec.size(); i++)
        {
            if (vec[i] == val1) {
                id1 = i;
            }
        }
        // cout << "in " << __FUNCTION__ << ": id1 = " << id1 << endl;
        if (id1 == 0   &&  vec[vec.size()-1] == val2) return true;
        if (id1 == 0)    return false;
        if (vec[id1-1] == val2) return true;

        return false;
    }


    /*--------------------------------------------------------------
     * Renvoie la valeur à la suite de @val1 dans le
     * tableau @vec .
     * Est circulaire. (ex: vec[0] peut être suivant de vec[fin])
     * -----------------------------------------------------------*/
    template<typename T>
    static T get_next_of(vector<T> vec, T val1)
    {
        if (!Utils::is_in_vector(vec, val1)) {
            cout << "in " << __FUNCTION__ << ": not in vector";
            return val1;
        }
        unsigned id1;
        for (unsigned i=0; i<vec.size(); i++)
        {
            if (vec[i] == val1) {
                id1 = i;
            }
        }
        // cout << "in " << __FUNCTION__ << ": id1 = " << id1 << endl;
        if (id1 == vec.size()-1) return vec[0];
        else return vec[id1+1];
    }

    /*--------------------------------------------------------------
     * Renvoie la valeur précédente de @val1 dans le
     * tableau @vec .
     * Est circulaire. (ex: vec[0] peut être suivant de vec[fin])
     * -----------------------------------------------------------*/
    template<typename T>
    static T get_pred_of(vector<T> vec, T val1)
    {
        if (!Utils::is_in_vector(vec, val1)) {
            cout << "in " << __FUNCTION__ << ": not in vector";
            return val1;
        }
        unsigned id1;
        for (unsigned i=0; i<vec.size(); i++)
        {
            if (vec[i] == val1) {
                id1 = i;
            }
        }
        // cout << "in " << __FUNCTION__ << ": id1 = " << id1 << endl;
        if (id1 == 0) return vec[vec.size()-1];
        else return vec[id1-1];
    }

};

#endif // UTILS_H
