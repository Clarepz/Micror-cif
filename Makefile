# Definitions de macros

CXX     = g++
CXXFLAGS = -g -Wall -std=c++17
CXXFILES = projet.cc simulation.cc shape.cc message.cc lifeform.cc gui.cc graphic.cc
OFILES = $(CXXFILES:.cc=.o)
LINKING = `pkg-config --cflags gtkmm-4.0`
LDLIBS = `pkg-config --libs gtkmm-4.0`

# Definition de la premiere regle

projet: $(OFILES)
	$(CXX) $(CXXFLAGS) $(LINKING) $(OFILES) -o $@ $(LDLIBS)


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
	@/bin/rm -f *.o *.x *.cc~ *.h~ projet

#
# -- Regles de dependances generees automatiquement
# -- ATTENTION Depend fait de la merde
# DO NOT DELETE THIS LINE
projet.o: projet.cc simulation.h lifeform.h shape.h graphic.h constantes.h gui.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
simulation.o: simulation.cc simulation.h lifeform.h shape.h graphic.h \
 constantes.h message.h
shape.o: shape.cc shape.h graphic.h
message.o: message.cc message.h
lifeform.o: lifeform.cc message.h lifeform.h shape.h graphic.h \
 constantes.h
gui.o: gui.cc gui.h graphic.h simulation.h lifeform.h shape.h constantes.h
	$(CXX) $(CXXFLAGS) $(LINKING) -c $< -o $@ $(LINKING)
graphic.o: graphic.cc graphic.h
