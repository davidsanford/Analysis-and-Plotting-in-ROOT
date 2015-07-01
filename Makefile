RC=$(shell root-config --cflags)
RL=$(shell root-config --libs)
SOURCE=src
OBJDIR=$(SOURCE)/obj
ExRoot=/opt/ExRootAnalysis/lib/libExRootAnalysis.so

EQDEP=$(SOURCE)/formula.h $(SOURCE)/limit.h $(SOURCE)/isospin.h

GRAPHDEP=$(SOURCE)/def.h $(SOURCE)/graph.h $(SOURCE)/curve.h \
$(SOURCE)/histogram.h $(SOURCE)/contour.h

PARSEDEP=$(SOURCE)/parse.h $(SOURCE)/parsecol.h $(SOURCE)/parselhco.h \
$(SOURCE)/event.h 

EQOBJ=$(OBJDIR)/formula.o $(OBJDIR)/limit.o $(OBJDIR)/isospin.o

GRAPHOBJ=$(OBJDIR)/def.o $(OBJDIR)/graph.o $(OBJDIR)/curve.o \
$(OBJDIR)/histogram.o $(OBJDIR)/contour.o

PARSEOBJ=$(OBJDIR)/parse.o $(OBJDIR)/parsecol.o $(OBJDIR)/parselhco.o \
$(SOURCE)/event.o

all : makeplots equation equationchecker numberlist

makeplots : $(SOURCE)/makeplots.cpp $(EQOBJ) $(GRAPHOBJ) $(PARSEOBJ) \
$(OBJDIR)/plot.o $(OBJDIR)/colgraph.o $(OBJDIR)/lhcograph.o \
$(OBJDIR)/parseslha.o $(OBJDIR)/aggregate.o $(OBJDIR)/convertcol.o
	g++ -o $@ $^ $(RL) $(RC)

equation : $(SOURCE)/equation.cpp $(EQOBJ)
	g++ -o $@ $^

equationchecker : $(SOURCE)/equationchecker.cpp $(EQOBJ)
	g++ -o $@ $^

numberlist: $(SOURCE)/numberlist.cpp
	g++ -o $@ $^

$(OBJDIR)/limit.o : $(SOURCE)/limit.cpp $(SOURCE)/limit.h
	g++ -c -o $@ $<

$(OBJDIR)/isospin.o : $(SOURCE)/isospin.cpp $(SOURCE)/isospin.h
	g++ -c -o $@ $<

$(OBJDIR)/formula.o : $(SOURCE)/formula.cpp $(EQDEP)
	g++ -c -o $@ $<

$(OBJDIR)/def.o : $(SOURCE)/def.cpp $(SOURCE)/def.h
	g++ -c -o $@ $<

$(OBJDIR)/event.o : $(SOURCE)/event.cpp $(SOURCE)/event.h $(SOURCE)/def.h
	g++ -c -o $@ $<

$(OBJDIR)/parse.o : $(SOURCE)/parse.cpp $(EQDEP) $(SOURCE)/def.h \
$(SOURCE)/parse.h
	g++ -c -o $@ $<

$(OBJDIR)/parsecol.o : $(SOURCE)/parsecol.cpp $(EQDEP) $(SOURCE)/def.h \
$(SOURCE)/parse.h $(SOURCE)/parsecol.h
	g++ -c -o $@ $<

$(OBJDIR)/parselhco.o : $(SOURCE)/parselhco.cpp $(EQDEP) $(SOURCE)/def.h \
$(SOURCE)/parselhco.h $(SOURCE)/parse.h $(SOURCE)/event.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/parseslha.o : $(SOURCE)/parseslha.cpp $(EQDEP) $(SOURCE)/def.h \
$(SOURCE)/parseslha.h $(SOURCE)/parse.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/graph.o : $(SOURCE)/graph.cpp $(SOURCE)/graph.h $(SOURCE)/def.h 
	g++ -c -o $@ $<

$(OBJDIR)/curve.o : $(SOURCE)/curve.cpp $(SOURCE)/curve.h \
$(SOURCE)/def.h  $(SOURCE)/graph.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/histogram.o: $(SOURCE)/histogram.cpp $(SOURCE)/histogram.h \
$(SOURCE)/def.h  $(SOURCE)/graph.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/contour.o : $(SOURCE)/contour.cpp $(SOURCE)/contour.h \
$(SOURCE)/def.h  $(SOURCE)/graph.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/colgraph.o : $(SOURCE)/colgraph.cpp $(SOURCE)/colgraph.h \
$(EQDEP) $(GRAPHDEP) $(SOURCE)/parse.h $(SOURCE)/parsecol.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/lhcograph.o : $(SOURCE)/lhcograph.cpp $(SOURCE)/lhcograph.h \
$(EQDEP) $(GRAPHDEP) $(SOURCE)/parse.h $(SOURCE)/parselhco.h \
$(SOURCE)/event.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/plot.o : $(SOURCE)/plot.cpp $(SOURCE)/plot.h $(EQDEP) \
$(GRAPHDEP) $(PARSEDEP) $(SOURCE)/colgraph.h $(SOURCE)/lhcograph.h \
$(SOURCE)/event.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/aggregate.o : $(SOURCE)/aggregate.cpp $(SOURCE)/aggregate.h \
$(EQDEP) $(PARSEDEP) $(SOURCE)/parseslha.h $(SOURCE)/def.h
	g++ $(RC) -c -o $@ $<

$(OBJDIR)/convertcol.o : $(SOURCE)/convertcol.cpp $(EQDEP) $(SOURCE)/def.h \
$(SOURCE)/parse.h $(SOURCE)/parsecol.h $(SOURCE)/convertcol.h
	g++ -c -o $@ $<

clean:
	rm *~ equationchecker makeplots aggregate checkfile */*~ $(OBJDIR)/*.o;
