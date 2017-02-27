#include "context.hpp"
//#include "toFile.hpp"
//#include <string>

//#include <fstream>
#include "stdlib.hpp"

int main() {
  uint n = 32;
  
  // New context
  CoreIRContext* c = newContext();
  
  //register the stdlib
  Namespace* std = registerStdlib(c);

  //Declare add2 Generator
  Generator* add2 = std->getGenerator("add2");

  // Define Add4 Module
  Type* add4Type = c->Record({
      {"in",c->Array(4,c->Array(n,c->BitIn()))},
      {"out",c->Array(n,c->BitOut())}
  });
  Module* add4_n = c->newModuleDecl("Add4",add4Type);
  ModuleDef* def = add4_n->newModuleDef();
    Wireable* iface = def->getInterface();
    Wireable* add_00 = def->addInstanceGenerator("add00",add2,new GenArgs(1,{c->GInt(n)}));
    Wireable* add_01 = def->addInstanceGenerator("add01",add2,new GenArgs(1,{c->GInt(n)}));
    Wireable* add_1 = def->addInstanceGenerator("add1",add2,new GenArgs(1,{c->GInt(n)}));
    
    def->wire(iface->sel("in")->sel(0),add_00->sel("in0"));
    def->wire(iface->sel("in")->sel(1),add_00->sel("in1"));
    def->wire(iface->sel("in")->sel(2),add_01->sel("in0"));
    def->wire(iface->sel("in")->sel(3),add_01->sel("in1"));

    def->wire(add_00->sel("out"),add_1->sel("in0"));
    def->wire(add_01->sel("out"),add_1->sel("in1"));

    def->wire(add_1->sel("out"),iface->sel("out"));
  // End Define Add4 Module
  add4_n->addModuleDef(def);

  add4_n->print();

  //Add Def to global
  c->getGlobal()->addModule(add4_n);
    
  // emit this circuit as a file
  //toFileMain(add4, "add4.core");

  //Link stdprim library
  
  //fstream f;
  //f.open("add4.v",fstream::out);
  //assert(f.is_open());
  //compile(c,add4,&f);
  //f.close();

  deleteContext(c);
  
  return 0;
}
