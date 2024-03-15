# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++11
CXXFILES = project.cc simulation.cc shape.cc message.cc lifeform.cc
OFILES = $(CXXFILES:.cc=.o)

# Definition de la premiere regle

project: $(OFILES)
	$(CXX) $(OFILES) -o project

# Definitions de cibles particulieres

depend:
	@echo " *** MISE A JOUR DES DEPENDANCES ***"
	@(sed '/^# DO NOT DELETE THIS LINE/q' Makefile && \
	  $(CXX) -MM $(CXXFLAGS) $(CXXFILES) | \
	  egrep -v "/usr/include" \
	 ) >Makefile.new
	@mv Makefile.new Makefile

clean:
	@echo " *** EFFACE MODULES OBJET ET EXECUTABLE ***"
	@/bin/rm -f *.o *.x *.cc~ *.h~ project

#
# -- Regles de dependances generees automatiquement
#
# DO NOT DELETE THIS LINE
project.o: project.cc simulation.h
simulation.o: simulation.cc simulation.h shape.h message.h
shape.o: shape.cc shape.h
message.o: message.cc message.h
lifeform.o: lifeform.cc lifeform.h
