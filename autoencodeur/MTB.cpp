/* 9/10
   - Il faut apprendre à exécuter avec redirection clavier.
   - Attention, <<= et >>= font des décalages, pas des rotations.
   */

#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <string>
using namespace std;

struct action_t {
    char write, direction, state;
    action_t(char w, char d, char s) : write(w), direction(d), state(s) {}
};

const int BITS = 16;

void print_ribbon(bitset<BITS>& ribbon, ofstream& output_file) {
    for (int i = BITS-1; i > 0; --i) {
        cout << ribbon[i] << " ";
        output_file << ribbon[i] << " ";
    }
    cout << endl;
    output_file << endl;
}

int main() {
    // Ouvrir le fichier d'entrée
    ifstream input_file("BTM.2in");
    if (!input_file) {
        cerr << "Erreur : Impossible d'ouvrir le fichier d'entrée." << endl;
        return 1;
    }

    // Lire le nombre d'états
    int n;
    input_file >> n;

    // Lire la table des actions
    vector<vector<action_t>> table(2, vector<action_t>(n, { '0', 'L', 'A' }));
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < n; ++col) {
            char write, direction, state;
            input_file >> write >> direction >> state;
            table[row][col] = { write, direction, state };
        }
    }

    input_file.close(); // Fermer le fichier après lecture

    // Initialisation du ruban et des variables
    bitset<BITS> ribbon; // Ruban initialisé à 0
    int head_position = 0; // Position initiale de la tête
    char current_state = 'A';

    // Ouvrir le fichier de sortie
    ofstream output_file("MTB.1out");
    if (!output_file) {
        cerr << "Erreur : Impossible de créer le fichier de sortie." << endl;
        return 1;
    }

    // Afficher l'état initial du ruban
    cout << "RIBBON: ";
    output_file << "RIBBON: ";
    print_ribbon(ribbon, output_file);

    // Simulation de la machine de Turing
    while (current_state != '#') {
        int state_index = current_state - 'A';
        int row = ribbon[head_position];
        action_t action = table[row][state_index];

        // Afficher et enregistrer l'action
        cout << "ACTION: {write " << action.write << ", move to the ";
        cout << (action.direction == 'L' ? "left" : "right") << ", new state " << action.state << "}" << endl;

        output_file << "ACTION: {write " << action.write << ", move to the ";
        output_file << (action.direction == 'L' ? "left" : "right") << ", new state " << action.state << "}" << endl;

        // Écrire sur le ruban
        ribbon[head_position] = action.write - '0';

        // Déplacer la tête
        if (action.direction == 'L') {
            if (head_position == 15) {
                // Décalage vers la droite
                ribbon >>= 1;
            }
            else {
                head_position++;
            }
        }
        else if (action.direction == 'R') {
            if (head_position == 0) {
                // Décalage vers la gauche
                ribbon <<= 1;
            }
            else {
                head_position--;
            }
        }

        // Mettre à jour l'état courant
        current_state = action.state;

        // Afficher et enregistrer l'état du ruban
        cout << "RIBBON: ";
        output_file << "RIBBON: ";
        print_ribbon(ribbon, output_file);
    }

    // Afficher et enregistrer la valeur décimale finale
    unsigned long decimal_value = ribbon.to_ulong();
    cout << decimal_value << endl;
    output_file << decimal_value << endl;

    output_file.close(); // Fermer le fichier de sortie
    return 0;
}
