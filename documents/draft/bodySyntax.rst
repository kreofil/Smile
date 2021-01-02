::
    Body = "{" Element {";" Element} [";"] "}".
    
    Element = Id "<<" Expression
            | Expression [">>" Id].
            
    Expression = Term { (":" Тerm) | PrefixExpression }.
    
    PrefixExpression = "^" Term [PrefixExpression].
    
    Term = Atom | QualId | TupleList | Block | Keywords(return...) | ...
    
    Atom = IntConst | BoolConst | FloatConst | "!".
    
    TupleList = "(" Expression {"," Expression} ")"
    
