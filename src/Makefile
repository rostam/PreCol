include ../../Makefile.incl

LIB = 
INC = -I../inc

requiredMEX = ISetColeman.o ISet.o ISetRestricted.o ISetVariant.o\
      ISetVariantRestricted.o kClique.o neighbors.o OrderingHeuristics.o PartialColoringRestricted.o\
      PartialD2Coloring.o PartialD2ColoringRestricted.o StarBicoloringSchemeCombinedVertexCoverColoring.o\
      StarBicoloringScheme.o StarBicoloringSchemeRestricted.o StarBicoloringSchemeDynamicOrdering.o \
      StarBicoloringSchemeDynamicOrderingRestricted.o StarBicoloringSchemeCombinedVertexCoverColoringRestricted.o

all: Coloring

Coloring: Coloring.cpp obj/Mtx2BipGraph.o obj/neighbors.o obj/PartialD2Coloring.o \
	obj/PartialD2ColoringRestricted.o obj/PartialColoringRestricted.o obj/OrderingHeuristics.o \
	obj/StarBicoloringScheme.o obj/StarBicoloringSchemeRestricted.o obj/StarBicoloringSchemeDynamicOrderingRestricted.o \
	obj/StarBicoloringSchemeCombinedVertexCoverColoring.o obj/StarBicoloringSchemeCombinedVertexCoverColoringRestricted.o obj/StarBicoloringSchemeDynamicOrdering.o \
	obj/ISet.o obj/ISetVariant.o obj/ISetColeman.o \
	obj/ISetRestricted.o obj/ISetVariantRestricted.o \
	obj/kClique.o obj/ConvertGraph.o \
	obj/output_graph.o
	$(CPP) $(CPP_FLAGS) $(LIB) $(INC) $< \
	obj/*.o \
	mmio.cpp \
	-o ../$@
	rm -f obj/mexobjects.flag

obj/%.o : %.cpp
	$(CPP) $(CPP_FLAGS) $(LIB) $(INC) -c $< -o $@

MEX: $(requiredMEX)
	touch obj/mexobjects.flag

$(requiredMEX): %.o: %.cpp
	$(CPP) -fPIC $(CPP_FLAGS) $(LIB) $(INC) -c $< -o obj/$@

clean :
	rm -f obj/*.o *~
