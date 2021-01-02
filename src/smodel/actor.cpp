#include <iostream>

#include "actor.h"
#include "funcdeclaration.h"

//==============================================================================
void Actor::DebugOut() {
    std::cout << number << ": ";
}

//==============================================================================
void ActorArg::DebugOut() {
    Actor::DebugOut();
    std::cout << "Argument @";
    if(DeclarationType* t = dynamic_cast<DeclarationType*>(GetOutDeclaration());
       t != nullptr) 
    {
        t->GetType()->DebugOut();
    } else {
        std::cout << "Incorrect DeclarationType";
    }
}

//==============================================================================
void ActorRet::DebugOut() {
    Actor::DebugOut();
    std::cout << "Return @";
    if(DeclarationType* t = dynamic_cast<DeclarationType*>(GetOutDeclaration());
       t != nullptr) 
    {
        t->GetType()->DebugOut();
    } else {
        std::cout << "Incorrect DeclarationType";
    }
}

//==============================================================================
void ActorConst::DebugOut() {
    Actor::DebugOut();
    std::cout << "Const ";
    if(DeclarationConst* t = dynamic_cast<DeclarationConst*>(GetOutDeclaration());
       t != nullptr) 
    {
        t->GetConst()->DebugOut();
    } else {
        std::cout << "Incorrect DeclarationType";
    }
}

//==============================================================================
void ActorInterpret::DebugOut() {
    Actor::DebugOut();
    std::cout << "One Arg Interp "
              << arg->GetNumber()
              << " : "
              << func->GetNumber();
}

//==============================================================================
void ActorInterpretMass::DebugOut() {
    Actor::DebugOut();
    std::cout << "Mass Arg Interp "
              << arg->GetNumber()
              << " : "
              << func->GetNumber();
}

//==============================================================================
void ActorFunc::DebugOut() {
    Actor::DebugOut();
    std::cout << "Func ";
    //DeclarationFunc* t = dynamic_cast<DeclarationFunc*>(GetOutDeclaration());
    if(DeclarationFunc* t = dynamic_cast<DeclarationFunc*>(GetOutDeclaration());
       t != nullptr) 
    {
        std::cout << t->GetName() << " << ";
        t->GetFuncProto()->DebugOut();
    } else {
        std::cout << "Incorrect DeclarationType";
    }
}

//==============================================================================
DeclarationActor::DeclarationActor(Actor* av): actorValue{av}
{}

DeclarationActor::DeclarationActor(std::string n, Actor* av):
    Declaration{n}, actorValue{av}
{}

// Формирование объявления как текстовой строки
std::string DeclarationActor::toString() {
    std::string s{"Actor name "};
    s += GetName();
    return s;
}

void DeclarationActor::DebugOut() {
}
