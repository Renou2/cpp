#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes e    Noeun fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstTq::NoeudInstTq(Noeud * cond, Noeud * seq) : m_cond(cond), m_seq(seq) {
    //
}

int NoeudInstTq::executer() {
    while (m_cond->executer()) {
        m_seq->executer();
    }
    return 0;
}

NoeudInstRep::NoeudInstRep(Noeud * cond, Noeud * seq) : m_cond(cond), m_seq(seq){
    
}

int NoeudInstRep::executer(){
    do {
        m_seq->executer();
    } while(!(m_cond->executer()));
    return 0;
}


NoeudInstPour::NoeudInstPour(Noeud * aff, Noeud* cond, Noeud * incr, Noeud * seq)
:m_cond(cond), m_seq(seq), m_incr(incr), m_aff(aff){
}


int NoeudInstPour::executer(){
    for ((m_aff == nullptr)? 0: m_aff->executer(); m_cond->executer(); (m_incr == nullptr)? 0: m_incr->executer()) {
        m_seq->executer();
    } 
    return 0;
}

NoeudInstSiRiche::NoeudInstSiRiche(Noeud* cond, Noeud* seq1, vector<Noeud*> & exps, vector<Noeud*> & seqs, Noeud* seq2 )
:m_cond1(cond), m_seq1(seq1), m_exps(exps), m_seqs(seqs), m_seq2(seq2)
{
    
}

int NoeudInstSiRiche::executer(){
    if (m_cond1->executer()) m_seq1->executer();
    for (int i = 0; i < m_exps.size(); i++) {
        if (m_exps[i]->executer()) {
            m_seqs[i]->executer();
        }
    }
    if (m_seq2 != nullptr) {
        m_seq2->executer();
    }
    return 0;
}