# Makefile for KDTree
# Inspired by: https://stackoverflow.com/questions/2481269/how-to-make-a-simple-c-makefile
# *****************************************************
# Variables to control Makefile operation
# $(shell root-config --ldflags)
CXX = g++
LDFLAGS=-g 
CXXFLAGS = -std=c++11 -Wall
RM = rm -f
# $(shell find . -maxdepth 1 -name "*.h")
HEADS=$(shell find . -maxdepth 1 -name "*.h")
# CKDTree.h ConvergenceExperiment.h CrackingStrategy.h CummulativeExperiment.h DataImporter.h DD1CStochasticCrackingStrategy.h DD1RKDTree.h DD1RStochasticCrackingStrategy.h Experiment.h ExperimentHelper.h ExperimentPlot.h FileWriter.h Helper.h KDNode.h KDTree.h LazyCKDTree.h LazyCrackingStrategy.h MDD1RStochasticCrackingStrategy.h ModularCKDTree.h PartitionSizeExperiment.h QUASII.h StaticKDTree.h StochasticCrackingStrategy.h
SRCS=$(subst .h,.cpp,$(HEADS))
OBJS=$(subst .h,.o,$(HEADS))
GCHS=$(subst .h,.h.gch,$(HEADS))


# targets for running code

run_experiments: experiments
	./experiments.out

run_test: test
	./test.out

run_kdtree: kdtree
	./test_kdtree.out

run_local: local
	./local_test.out

# targets for creating executables

test: Test.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test.out Test.cpp $(OBJS) -lcriterion

experiments: experiments.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o experiments.out experiments.cpp $(OBJS)

kdtree: test_kdtree.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o test_kdtree.out test_kdtree.cpp $(OBJS)

local: local_test.o $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o local_test.out local_test.cpp $(OBJS)

# targets for compiling source code

# %.o: %.h
# 	$(CXX) $(CXXFLAGS) -c $(subst .h,.cpp,$<) $<

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

# utility targets

clean: clean_gch
	$(RM) experiments.o Test.o test_kdtree.o $(OBJS)

clean_gch:
	$(RM) $(GCHS)


distclean: clean
	$(RM) *~ .depend

include .depend
