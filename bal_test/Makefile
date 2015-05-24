CC = gcc
BAL_PATH = ../BAL
CFLAGS = -g -ggdb -I$(BAL_PATH)
LDFLAGS = -L$(BAL_PATH) -lbal `pkg-config --cflags --libs glib-2.0` -lfl -lm
RM = rm -f
TESTS = coord_test \
		mult_vec_packcol_symmetric_test \
		mult_vec_packcol_test \
		packcol_symmetric_test \
		packcol_test \
		parser_test \
		row_traversal_packcol_test \
		elimination_tree_test \
		symbolic_factorization_test \
		numerical_factorization_test \
		save_packcol_symmetric_test \
		cholesky_Lsolver_test \
		cholesky_LTsolver_test \
		cholesky_solver_test \
		cds_test \
		mult_vec_cds_test \
		mult_mat_cds_test \
		save_cds_test \
		permutar_packcol_test \
		save_coord_test \
		load_coord_test

all: $(TESTS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	$(RM) $(TESTS) *.out *.error

