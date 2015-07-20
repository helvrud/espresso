#ifndef REACTION_ENSEMBLE_H
#define REACTION_ENSEMBLE_H

#include "utils.hpp"

typedef struct single_reaction{
	//strict input to the algorithm
	int* educt_types;
	int len_educt_types;
	int* educt_coefficients;
	int* product_types;
	int len_product_types;
	int* product_coefficients;
	double equilibrium_constant;
	//calculated values that are stored for performance reasons
	int nu_bar;
}  single_reaction;

typedef struct reaction_system {
	int nr_single_reactions;
	single_reaction** reactions;
	float volume;
	int* type_index;
	int nr_different_types; // is equal to length type_index
	double* charges_of_types;
	int water_type; //TODO needs to be used
	double standard_pressure_in_simulation_units;
} reaction_system;

extern reaction_system current_reaction_system;

int do_reaction();

int create_current_reaction_system_struct();

int free_reaction_ensemble();

int initialize();

int calculate_nu_bar(int* educt_coefficients, int len_educt_types,  int* product_coefficients, int len_product_types);

int update_type_index(int* educt_types, int len_educt_types , int* product_types, int len_product_types); //assign different types an index in a growing list that starts at 0 and is incremented by 1 for each new type. the entry in the index at place i is the "type_value". therefore the type of type "typevalue" has the index i; 

int generic_oneway_reaction(int reaction_id);

int find_index_of_type(int type);

///////////////////////////////////////////// Wang-Landau algorithm

typedef struct collective_variable{
	double CV_minimum;
	double CV_maximum;
	double delta_CV;
	int* corresponding_acid_types; // is NULL if the current collective variable has nothing to do with a degree of association
	int nr_corresponding_acid_types;
	int associated_type;
	double (*determine_current_state_in_collective_variable_with_index) (int);	//declare a function pointer with name determine_current_state_in_this_collective_variable that has to point to a function that has 1 int input parameter (for the collective_variable_index) and returns a double
} collective_variable;

typedef struct wang_landau_system {
	int* histogram;
	int len_histogram; //equals also len_wang_landau_potential and also len_Gamma
	double* wang_landau_potential; //equals the logarithm to basis e of the degeneracy of the states
	int nr_collective_variables;
	collective_variable** collective_variables;
	double wang_landau_parameter; //equals the logarithm to basis e of the modification factor of the degeneracy of states when the state is visited
	double initial_wang_landau_parameter;
	int already_refined_n_times;
	
	int int_fill_value;
	double double_fill_value;
	
	int number_of_monte_carlo_moves_between_check_of_convergence;
	double final_wang_landau_parameter;
	int monte_carlo_trial_moves; //for 1/t algorithm

	int wang_landau_relaxation_steps; //relaxation steps like relaxation steps for metadynamics in order to find local equilibrium
	char* output_filename;
} wang_landau_system;

extern wang_landau_system current_wang_landau_system;

int initialize_wang_landau(); //may first be called after all collective variables are added
int do_reaction_wang_landau();
void free_wang_landau();

#endif /* ifdef REACTION_H */