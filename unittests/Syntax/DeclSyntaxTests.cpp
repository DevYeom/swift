#include "swift/Syntax/SyntaxFactory.h"
#include "swift/Syntax/SyntaxNodes.h"
#include "swift/Syntax/SyntaxBuilders.h"
#include "llvm/ADT/SmallString.h"
#include "gtest/gtest.h"

using llvm::None;
using llvm::SmallString;

using namespace swift;
using namespace swift::syntax;

#pragma mark - declaration-modifier

DeclModifierSyntax getCannedDeclModifier(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto Private = Factory.makeIdentifier("private", "", "");
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Set = Factory.makeIdentifier("set", "", "");
  auto RParen = Factory.makeRightParenToken("", "");
  auto DeclModifierDetail = Factory.makeDeclModifierDetail(
      /*UnexpectedNodes=*/None, LParen, /*UnexpectedNodes=*/None, Set,
      /*UnexpectedNodes=*/None, RParen);
  return Factory.makeDeclModifier(/*UnexpectedNodes=*/None, Private,
                                  /*UnexpectedNodes=*/None, DeclModifierDetail);
}

TEST(DeclSyntaxTests, DeclModifierMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  {
    SyntaxFactory Factory(Arena);
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankDeclModifier().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<24> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedDeclModifier(Arena).print(OS);
    ASSERT_EQ(OS.str().str(), "private(set)");
  }
}

TEST(DeclSyntaxTests, DeclModifierGetAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto Private = Factory.makeIdentifier("private", "", "");
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Set = Factory.makeIdentifier("set", "", "");
  auto RParen = Factory.makeRightParenToken("", "");
  auto DeclModifierDetail = Factory.makeDeclModifierDetail(
      /*UnexpectedNodes=*/None, LParen, /*UnexpectedNodes=*/None, Set,
      /*UnexpectedNodes=*/None, RParen);
  auto Mod =
      Factory.makeDeclModifier(/*UnexpectedNodes=*/None, Private,
                               /*UnexpectedNodes=*/None, DeclModifierDetail);

  ASSERT_EQ(Private.getRaw(), Mod.getName().getRaw());
  ASSERT_EQ(LParen.getRaw(), Mod.getDetail()->getLeftParen().getRaw());
  ASSERT_EQ(Set.getRaw(), Mod.getDetail()->getDetail().getRaw());
  ASSERT_EQ(RParen.getRaw(), Mod.getDetail()->getRightParen().getRaw());
}

TEST(DeclSyntaxTests, DeclModifierWithAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto Private = Factory.makeIdentifier("private", "", "");
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Set = Factory.makeIdentifier("set", "", "");
  auto RParen = Factory.makeRightParenToken("", "");

  SmallString<24> Scratch;
  llvm::raw_svector_ostream OS(Scratch);
  Factory.makeBlankDeclModifier()
      .withName(Private)
      .withDetail(Factory.makeDeclModifierDetail(/*UnexpectedNodes=*/None, LParen,
                                                 /*UnexpectedNodes=*/None, Set,
                                                 /*UnexpectedNodes=*/None, RParen))
      .print(OS);
  ASSERT_EQ(OS.str().str(), "private(set)");
}

#pragma mark - typealias-decl

TEST(DeclSyntaxTests, TypealiasMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankTypealiasDecl().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<64> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    auto Typealias = Factory.makeTypealiasKeyword("", " ");
    auto Subsequence = Factory.makeIdentifier("MyCollection", "", "");
    auto ElementName = Factory.makeIdentifier("Element", "", "");
    auto ElementParam = Factory.makeGenericParameter(
        /*UnexpectedNodes=*/None, /*Attributes=*/None, /*UnexpectedNodes=*/None,
        /*Name=*/ElementName, /*UnexpectedNodes=*/None, /*Colon=*/None,
        /*UnexpectedNodes=*/None, /*InheritedType=*/None, /*UnexpectedNodes=*/None,
        /*TrailingComma=*/None);
    auto LeftAngle = Factory.makeLeftAngleToken("", "");
    auto RightAngle = Factory.makeRightAngleToken("", " ");
    auto GenericParams = GenericParameterClauseSyntaxBuilder(Arena)
                             .useLeftAngleBracket(LeftAngle)
                             .useRightAngleBracket(RightAngle)
                             .addGenericParameter(ElementParam)
                             .build();
    auto Assignment = Factory.makeEqualToken("", " ");
    auto ElementType = Factory.makeTypeIdentifier("Element", "", "");
    auto ElementArg = Factory.makeGenericArgument(
        /*UnexpectedNodes=*/None, /*ArgumentType=*/ElementType,
        /*UnexpectedNodes=*/None, /*TrailingComma=*/None);

    auto GenericArgs =
        GenericArgumentClauseSyntaxBuilder(Arena)
            .useLeftAngleBracket(LeftAngle)
            .useRightAngleBracket(Factory.makeRightAngleToken("", ""))
            .addArgument(ElementArg)
            .build();

    auto Array = Factory.makeIdentifier("Array", "", "");
    auto Array_Int = Factory.makeSimpleTypeIdentifier(
        /*UnexpectedNodes=*/None, /*Name=*/Array, /*UnexpectedNodes=*/None,
        /*GenericArgumentClause=*/GenericArgs);
    auto TypeInit = Factory.makeTypeInitializerClause(
        /*UnexpectedNodes=*/None, /*Equal=*/Assignment, /*UnexpectedNodes=*/None,
        /*Value=*/Array_Int);
    Factory
        .makeTypealiasDecl(
            /*UnexpectedNodes=*/None, /*Attributes=*/None, /*UnexpectedNodes=*/None,
            /*Modifiers=*/None, /*UnexpectedNodes=*/None,
            /*TypealiasKeyword=*/Typealias, /*UnexpectedNodes=*/None,
            /*Identifier=*/Subsequence, /*UnexpectedNodes=*/None,
            /*GenericParameterClause=*/GenericParams, /*UnexpectedNodes=*/None,
            /*Initializer=*/TypeInit, /*UnexpectedNodes=*/None,
            /*GenericWhereClause=*/None)
        .print(OS);
    ASSERT_EQ(OS.str().str(),
              "typealias MyCollection<Element> = Array<Element>");
  }
}

TEST(DeclSyntaxTests, TypealiasWithAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto Typealias = Factory.makeTypealiasKeyword("", " ");
  auto MyCollection = Factory.makeIdentifier("MyCollection", "", "");
  auto ElementName = Factory.makeIdentifier("Element", "", "");
  auto ElementParam = Factory.makeGenericParameter(
      /*UnexpectedNodes=*/None, None, /*UnexpectedNodes=*/None, ElementName,
      /*UnexpectedNodes=*/None, None, /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, None);
  auto LeftAngle = Factory.makeLeftAngleToken("", "");
  auto RightAngle = Factory.makeRightAngleToken("", " ");
  auto GenericParams = GenericParameterClauseSyntaxBuilder(Arena)
                           .useLeftAngleBracket(LeftAngle)
                           .useRightAngleBracket(RightAngle)
                           .addGenericParameter(ElementParam)
                           .build();
  auto Equal = Factory.makeEqualToken("", " ");

  auto ElementType = Factory.makeTypeIdentifier("Element", "", "");
  auto ElementArg = Factory.makeGenericArgument(
      /*UnexpectedNodes=*/None, ElementType, /*UnexpectedNodes=*/None, None);
  auto GenericArgs =
      GenericArgumentClauseSyntaxBuilder(Arena)
          .useLeftAngleBracket(LeftAngle)
          .useRightAngleBracket(Factory.makeRightAngleToken("", ""))
          .addArgument(ElementArg)
          .build();

  auto Array = Factory.makeIdentifier("Array", "", "");
  auto Array_Int = Factory.makeSimpleTypeIdentifier(
      /*UnexpectedNodes=*/None, Array, /*UnexpectedNodes=*/None, GenericArgs);
  auto Type_Init = Factory.makeTypeInitializerClause(
      /*UnexpectedNodes=*/None, Equal, /*UnexpectedNodes=*/None, Array_Int);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankTypealiasDecl()
        .withTypealiasKeyword(Typealias)
        .withIdentifier(MyCollection)
        .withGenericParameterClause(GenericParams)
        .withInitializer(Type_Init)
        .print(OS);
    ASSERT_EQ(OS.str().str(),
              "typealias MyCollection<Element> = Array<Element>");
  }
}

TEST(DeclSyntaxTests, TypealiasBuilderAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  SmallString<64> Scratch;
  llvm::raw_svector_ostream OS(Scratch);

  auto Typealias = Factory.makeTypealiasKeyword("", " ");
  auto MyCollection = Factory.makeIdentifier("MyCollection", "", "");
  auto ElementName = Factory.makeIdentifier("Element", "", "");
  auto ElementParam = Factory.makeGenericParameter(ElementName, None);
  auto LeftAngle = Factory.makeLeftAngleToken("", "");
  auto RightAngle = Factory.makeRightAngleToken("", " ");
  auto GenericParams = GenericParameterClauseSyntaxBuilder(Arena)
                           .useLeftAngleBracket(LeftAngle)
                           .useRightAngleBracket(RightAngle)
                           .addGenericParameter(ElementParam)
                           .build();
  auto Equal = Factory.makeEqualToken("", " ");

  auto ElementType = Factory.makeTypeIdentifier("Element", "", "");
  auto ElementArg = Factory.makeGenericArgument(
      /*UnexpectedNodes=*/None, ElementType, /*UnexpectedNodes=*/None, None);

  auto GenericArgs =
      GenericArgumentClauseSyntaxBuilder(Arena)
          .useLeftAngleBracket(LeftAngle)
          .useRightAngleBracket(Factory.makeRightAngleToken("", ""))
          .addArgument(ElementArg)
          .build();

  auto Array = Factory.makeIdentifier("Array", "", "");
  auto Array_Int = Factory.makeSimpleTypeIdentifier(
      /*UnexpectedNodes=*/None, Array, /*UnexpectedNodes=*/None, GenericArgs);
  auto Type_Init = Factory.makeTypeInitializerClause(
      /*UnexpectedNodes=*/None, Equal, /*UnexpectedNodes=*/None, Array_Int);
  TypealiasDeclSyntaxBuilder(Arena)
      .useTypealiasKeyword(Typealias)
      .useIdentifier(MyCollection)
      .useGenericParameterClause(GenericParams)
      .useInitializer(Type_Init)
      .build()
      .print(OS);
  ASSERT_EQ(OS.str().str(),
            "typealias MyCollection<Element> = Array<Element>");
}

#pragma mark - parameter

FunctionParameterSyntax getCannedFunctionParameter(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto ExternalName = Factory.makeIdentifier("with", "", " ");
  auto LocalName = Factory.makeIdentifier("radius", "", "");
  auto Colon = Factory.makeColonToken("", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", " ");
  auto NoEllipsis = TokenSyntax::missingToken(tok::identifier, "...", Arena);
  auto Equal = Factory.makeEqualToken("", " ");

  auto Sign = Factory.makePrefixOperator("-", "", "");
  auto OneDigits = Factory.makeIntegerLiteral("1", "", "");
  auto OneLiteral =
      Factory.makeIntegerLiteralExpr(/*UnexpectedNodes=*/None, OneDigits);
  auto One = Factory.makePrefixOperatorExpr(/*UnexpectedNodes=*/None, Sign,
                                            /*UnexpectedNodes=*/None, OneLiteral);
  auto DefaultArg = Factory.makeInitializerClause(/*UnexpectedNodes=*/None, Equal,
                                                  /*UnexpectedNodes=*/None, One);
  auto Comma = Factory.makeCommaToken("", " ");

  return Factory.makeFunctionParameter(
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, ExternalName,
      /*UnexpectedNodes=*/None, LocalName, /*UnexpectedNodes=*/None, Colon,
      /*UnexpectedNodes=*/None, Int, /*UnexpectedNodes=*/None, NoEllipsis,
      /*UnexpectedNodes=*/None, DefaultArg, /*UnexpectedNodes=*/None, Comma);
}

TEST(DeclSyntaxTests, FunctionParameterMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionParameter(Arena).print(OS);
    ASSERT_EQ(OS.str().str(), "with radius: Int = -1, ");
  }
  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankFunctionParameter().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
}

TEST(DeclSyntaxTests, FunctionParameterGetAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto ExternalName = Factory.makeIdentifier("with", "", " ");
  auto LocalName = Factory.makeIdentifier("radius", "", "");
  auto Colon = Factory.makeColonToken("", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", " ");
  auto NoEllipsis = TokenSyntax::missingToken(tok::identifier, "...", Arena);
  auto Equal = Factory.makeEqualToken("", " ");

  auto Sign = Factory.makePrefixOperator("-", "", "");
  auto OneDigits = Factory.makeIntegerLiteral("1", "", "");
  auto OneLiteral =
      Factory.makeIntegerLiteralExpr(/*UnexpectedNodes=*/None, OneDigits);
  auto One = Factory.makePrefixOperatorExpr(/*UnexpectedNodes=*/None, Sign,
                                            /*UnexpectedNodes=*/None, OneLiteral);
  auto DefaultArg = Factory.makeInitializerClause(/*UnexpectedNodes=*/None, Equal,
                                                  /*UnexpectedNodes=*/None, One);
  auto Comma = Factory.makeCommaToken("", "");

  auto Param = Factory.makeFunctionParameter(
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, ExternalName,
      /*UnexpectedNodes=*/None, LocalName, /*UnexpectedNodes=*/None, Colon,
      /*UnexpectedNodes=*/None, Int, /*UnexpectedNodes=*/None, NoEllipsis,
      /*UnexpectedNodes=*/None, DefaultArg, /*UnexpectedNodes=*/None, Comma);

  ASSERT_EQ(ExternalName.getRaw(), Param.getFirstName()->getRaw());
  ASSERT_EQ(LocalName.getRaw(), Param.getSecondName()->getRaw());
  ASSERT_EQ(Colon.getRaw(), Param.getColon()->getRaw());

  auto GottenType = Param.getType();
  auto GottenType2 = Param.getType();
  ASSERT_TRUE(GottenType->hasSameIdentityAs(*GottenType2));

  ASSERT_EQ(DefaultArg.getRaw(), Param.getDefaultArgument()->getRaw());

  auto GottenDefaultValue = Param.getDefaultArgument()->getValue();
  auto GottenDefaultValue2 = Param.getDefaultArgument()->getValue();
  ASSERT_TRUE(GottenDefaultValue.hasSameIdentityAs(GottenDefaultValue2));

  ASSERT_EQ(Comma.getRaw(), Param.getTrailingComma()->getRaw());

  // Test that llvm::None is returned for non-token missing children:
  auto Decimated = Param
    .withType(llvm::None)
    .withDefaultArgument(llvm::None);

  ASSERT_FALSE(Decimated.getType().hasValue());
  ASSERT_FALSE(Decimated.getDefaultArgument().hasValue());
}

TEST(DeclSyntaxTests, FunctionParameterWithAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto ExternalName = Factory.makeIdentifier("for", "", " ");
  auto LocalName = Factory.makeIdentifier("integer", "", "");
  auto Colon = Factory.makeColonToken(" ", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", " ");
  auto Equal = Factory.makeEqualToken("", " ");

  auto NoSign = TokenSyntax::missingToken(tok::oper_prefix, "", Arena);
  auto OneDigits = Factory.makeIntegerLiteral("1", "", "");
  auto One = Factory.makeIntegerLiteralExpr(/*UnexpectedNodes=*/None, OneDigits);
  auto DefaultArg = Factory.makeInitializerClause(/*UnexpectedNodes=*/None, Equal,
                                                  /*UnexpectedNodes=*/None, One);
  auto Comma = Factory.makeCommaToken("", "");

  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionParameter(Arena)
      .withFirstName(ExternalName)
      .withSecondName(LocalName)
      .withColon(Colon)
      .withType(Int)
      .withDefaultArgument(DefaultArg)
      .withTrailingComma(Comma)
      .print(OS);
    ASSERT_EQ(OS.str().str(), "for integer : Int = 1,");
  }
  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionParameter(Arena)
      .withType(llvm::None)
      .withDefaultArgument(llvm::None)
      .print(OS);
    ASSERT_EQ(OS.str().str(), "with radius: , ");
  }
}

TEST(DeclSyntaxTests, FunctionParameterWithEllipsis) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto ExternalName = Factory.makeIdentifier("for", "", " ");
  auto LocalName = Factory.makeIdentifier("integer", "", "");
  auto Colon = Factory.makeColonToken(" ", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", "");
  auto Ellipsis = Factory.makeEllipsisToken("", " ");
  auto Comma = Factory.makeCommaToken("", "");

  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionParameter(Arena)
        .withFirstName(ExternalName)
        .withSecondName(LocalName)
        .withColon(Colon)
        .withType(Int)
        .withEllipsis(Ellipsis)
        .withDefaultArgument(llvm::None)
        .withTrailingComma(Comma)
        .print(OS);
    ASSERT_EQ(OS.str().str(), "for integer : Int... ,");
  }
}

#pragma mark - parameter-list

TEST(DeclSyntaxTests, FunctionParameterListMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankFunctionParameterList().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    auto Param = getCannedFunctionParameter(Arena);
    std::vector<FunctionParameterSyntax> Params { Param, Param, Param };
    Factory.makeFunctionParameterList(Params).print(OS);
    ASSERT_EQ(OS.str().str(),
      "with radius: Int = -1, with radius: Int = -1, with radius: Int = -1, ");
  }
}

#pragma mark - function-signature

FunctionSignatureSyntax getCannedFunctionSignature(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Param = getCannedFunctionParameter(Arena);
  auto List = Factory.makeBlankFunctionParameterList()
                  .appending(Param)
                  .appending(Param)
                  .appending(Param)
                  .castTo<FunctionParameterListSyntax>();
  auto RParen = Factory.makeRightParenToken("", " ");
  auto Parameter = Factory.makeParameterClause(/*UnexpectedNodes=*/None, LParen,
                                               /*UnexpectedNodes=*/None, List,
                                               /*UnexpectedNodes=*/None, RParen);
  auto Throws = Factory.makeThrowsKeyword("", " ");
  auto Arrow = Factory.makeArrowToken("", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", " ");
  auto Return = Factory.makeReturnClause(/*UnexpectedNodes=*/None, Arrow,
                                         /*UnexpectedNodes=*/None, Int);

  return Factory.makeFunctionSignature(
      /*UnexpectedNodes=*/None, Parameter, /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, Throws, /*UnexpectedNodes=*/None, Return);
}

TEST(DeclSyntaxTests, FunctionSignatureMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankFunctionSignature().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionSignature(Arena).print(OS);
    ASSERT_EQ(OS.str().str(),
      "(with radius: Int = -1, "
      "with radius: Int = -1, "
      "with radius: Int = -1, ) throws -> Int ");
  }
}

TEST(DeclSyntaxTests, FunctionSignatureGetAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Param = getCannedFunctionParameter(Arena);
  auto List = Factory.makeBlankFunctionParameterList()
                  .appending(Param)
                  .appending(Param)
                  .appending(Param)
                  .castTo<FunctionParameterListSyntax>();
  auto RParen = Factory.makeRightParenToken("", " ");
  auto Throws = Factory.makeThrowsKeyword("", " ");
  auto Arrow = Factory.makeArrowToken("", " ");

  auto Int = Factory.makeTypeIdentifier("Int", "", "");

  auto ParamClause = Factory.makeParameterClause(/*UnexpectedNodes=*/None, LParen,
                                                 /*UnexpectedNodes=*/None, List,
                                                 /*UnexpectedNodes=*/None, RParen);
  auto ReturnClause = Factory.makeReturnClause(/*UnexpectedNodes=*/None, Arrow,
                                               /*UnexpectedNodes=*/None, Int);
  auto Sig = Factory.makeFunctionSignature(
      /*UnexpectedNodes=*/None, ParamClause, /*UnexpectedNodes=*/None, None,
      /*UnexpectedNodes=*/None, Throws, /*UnexpectedNodes=*/None, ReturnClause);

  ASSERT_EQ(LParen.getRaw(), Sig.getInput().getLeftParen().getRaw());

  {
    SmallString<48> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    auto GottenList1 = Sig.getInput().getParameterList();
    auto GottenList2 = Sig.getInput().getParameterList();
    ASSERT_TRUE(GottenList1.hasSameIdentityAs(GottenList2));
    GottenList1.print(OS);
    ASSERT_EQ(OS.str().str(),
              "with radius: Int = -1, "
              "with radius: Int = -1, "
              "with radius: Int = -1, ");
  }

  ASSERT_EQ(RParen.getRaw(), Sig.getInput().getRightParen().getRaw());
  ASSERT_EQ(Throws.getRaw(), Sig.getThrowsOrRethrowsKeyword()->getRaw());
  ASSERT_EQ(Sig.getThrowsOrRethrowsKeyword()->getTokenKind(), tok::kw_throws);
  ASSERT_EQ(Arrow.getRaw(), Sig.getOutput()->getArrow().getRaw());

  {
    SmallString<3> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    auto GottenReturnType1 = Sig.getOutput()->getReturnType();
    auto GottenReturnType2 = Sig.getOutput()->getReturnType();
    ASSERT_TRUE(GottenReturnType1.hasSameIdentityAs(GottenReturnType2));
    GottenReturnType1.print(OS);
    ASSERT_EQ(OS.str().str(), "Int");
  }
}

TEST(DeclSyntaxTests, FunctionSignatureWithAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  auto LParen = Factory.makeLeftParenToken("", "");
  auto Param = getCannedFunctionParameter(Arena);
  auto List = Factory.makeBlankFunctionParameterList()
                  .appending(Param)
                  .appending(Param)
                  .appending(Param)
                  .castTo<FunctionParameterListSyntax>();
  auto RParen = Factory.makeRightParenToken("", " ");
  auto Throws = Factory.makeThrowsKeyword("", " ");
  auto Arrow = Factory.makeArrowToken("", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", "");

  auto Parameter = Factory.makeParameterClause(/*UnexpectedNodes=*/None, LParen,
                                               /*UnexpectedNodes=*/None, List,
                                               /*UnexpectedNodes=*/None, RParen);
  auto Return = Factory.makeReturnClause(/*UnexpectedNodes=*/None, Arrow,
                                         /*UnexpectedNodes=*/None, Int);
  SmallString<48> Scratch;
  llvm::raw_svector_ostream OS(Scratch);
  Factory.makeBlankFunctionSignature()
      .withInput(Parameter)
      .withThrowsOrRethrowsKeyword(Throws)
      .withOutput(Return)
      .print(OS);
  ASSERT_EQ(OS.str().str(),
            "(with radius: Int = -1, "
            "with radius: Int = -1, "
            "with radius: Int = -1, ) throws -> Int");
}

#pragma mark - function-declaration

ModifierListSyntax getCannedModifiers(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto PublicID = Factory.makePublicKeyword("", " ");
  auto NoLParen = TokenSyntax::missingToken(tok::l_paren, "(", Arena);
  auto NoArgument = TokenSyntax::missingToken(tok::identifier, "", Arena);
  auto NoRParen = TokenSyntax::missingToken(tok::r_paren, ")", Arena);
  auto PublicDeclModifierDetail = Factory.makeDeclModifierDetail(
      /*UnexpectedNodes=*/None, NoLParen, /*UnexpectedNodes=*/None, NoArgument,
      /*UnexpectedNodes=*/None, NoRParen);
  auto Public = Factory.makeDeclModifier(
      /*UnexpectedNodes=*/None, PublicID,
      /*UnexpectedNodes=*/None, PublicDeclModifierDetail);

  auto StaticKW = Factory.makeStaticKeyword("", " ");
  auto StaticDeclModifierDetail = Factory.makeDeclModifierDetail(
      /*UnexpectedNodes=*/None, NoLParen, /*UnexpectedNodes=*/None, NoArgument,
      /*UnexpectedNodes=*/None, NoRParen);
  auto Static =
      Factory.makeDeclModifier(/*UnexpectedNodes=*/None, StaticKW,
                               /*UnexpectedNodes=*/None, StaticDeclModifierDetail);

  return Factory.makeBlankModifierList().appending(Public).appending(Static);
}

GenericParameterClauseSyntax getCannedGenericParams(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  GenericParameterClauseSyntaxBuilder GB(Arena);

  auto LAngle = Factory.makeLeftAngleToken("", "");
  auto RAngle = Factory.makeRightAngleToken("", "");
  auto TType = Factory.makeIdentifier("T", "", "");
  auto UType = Factory.makeIdentifier("U", "", "");

  auto Comma = Factory.makeCommaToken("", " ");
  auto T = Factory.makeGenericParameter(TType, Comma);
  auto U = Factory.makeGenericParameter(UType, None);

  GB.addGenericParameter(T);
  GB.addGenericParameter(U);
  GB.useLeftAngleBracket(LAngle);
  GB.useRightAngleBracket(RAngle);

  return GB.build();
}

CodeBlockSyntax getCannedBody(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto NoSign = TokenSyntax::missingToken(tok::oper_prefix, "-", Arena);
  auto OneDigits = Factory.makeIntegerLiteral("1", "", "");
  auto One = Factory.makeIntegerLiteralExpr(/*UnexpectedNodes=*/None, OneDigits);
  auto ReturnKW = Factory.makeReturnKeyword("\n  ", "");
  auto Return = Factory.makeReturnStmt(/*UnexpectedNodes=*/None, ReturnKW,
                                       /*UnexpectedNodes=*/None, One);
  auto ReturnItem = Factory.makeCodeBlockItem(/*UnexpectedNodes=*/None, Return,
                                              /*UnexpectedNodes=*/None, None,
                                              /*UnexpectedNodes=*/None, None);

  auto Stmts = Factory.makeCodeBlockItemList({ReturnItem});

  auto LBrace = Factory.makeLeftBraceToken("", "");
  auto RBrace = Factory.makeRightBraceToken("\n", "");

  return Factory.makeCodeBlock(/*UnexpectedNodes=*/None, LBrace,
                               /*UnexpectedNodes=*/None, Stmts,
                               /*UnexpectedNodes=*/None, RBrace);
}

GenericWhereClauseSyntax getCannedWhereClause(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto WhereKW = Factory.makeWhereKeyword("", " ");
  auto T = Factory.makeTypeIdentifier("T", "", " ");
  auto EqualEqual = Factory.makeEqualityOperator("", " ");
  auto Int = Factory.makeTypeIdentifier("Int", "", " ");
  auto SameType = Factory.makeSameTypeRequirement(
      /*UnexpectedNodes=*/None, T, /*UnexpectedNodes=*/None, EqualEqual,
      /*UnexpectedNodes=*/None, Int);
  auto Req = Factory.makeGenericRequirement(/*UnexpectedNodes=*/None, SameType,
                                            /*UnexpectedNodes=*/None, None);

  auto Requirements = Factory.makeBlankGenericRequirementList().appending(Req);

  return Factory.makeBlankGenericWhereClause()
      .withWhereKeyword(WhereKW)
      .withRequirementList(Requirements);
}

FunctionDeclSyntax getCannedFunctionDecl(const RC<SyntaxArena> &Arena) {
  SyntaxFactory Factory(Arena);
  auto NoAttributes = Factory.makeBlankAttributeList();
  auto Foo = Factory.makeIdentifier("foo", "", "");
  auto FuncKW = Factory.makeFuncKeyword("", " ");
  auto Modifiers = getCannedModifiers(Arena);
  auto GenericParams = getCannedGenericParams(Arena);
  auto GenericWhere = getCannedWhereClause(Arena);
  auto Signature = getCannedFunctionSignature(Arena);
  auto Body = getCannedBody(Arena);

  return Factory.makeFunctionDecl(
      /*UnexpectedNodes=*/None, NoAttributes, /*UnexpectedNodes=*/None, Modifiers,
      /*UnexpectedNodes=*/None, FuncKW, /*UnexpectedNodes=*/None, Foo,
      /*UnexpectedNodes=*/None, GenericParams, /*UnexpectedNodes=*/None, Signature,
      /*UnexpectedNodes=*/None, GenericWhere, /*UnexpectedNodes=*/None, Body);
}

TEST(DeclSyntaxTests, FunctionDeclMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankFunctionDecl().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<64> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    getCannedFunctionDecl(Arena).print(OS);
    ASSERT_EQ(OS.str().str(),
              "public static func foo<T, U>"
              "(with radius: Int = -1, "
              "with radius: Int = -1, "
              "with radius: Int = -1, ) "
              "throws -> Int "
              "where T == Int {\n"
              "  return1\n"
              "}");
  }
}

TEST(DeclSyntaxTests, FunctionDeclGetAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankFunctionDecl().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {

  }
}

TEST(DeclSyntaxTests, FunctionDeclWithAPIs) {

}

TEST(DeclSyntaxTests, FunctionDeclBuilderAPIs) {

}

#pragma mark - parameterized protocol-decl

TEST(DeclSyntaxTests, ProtocolMakeAPIs) {
  RC<SyntaxArena> Arena = SyntaxArena::make();
  SyntaxFactory Factory(Arena);
  {
    SmallString<1> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    Factory.makeBlankProtocolDecl().print(OS);
    ASSERT_EQ(OS.str().str(), "");
  }
  {
    SmallString<64> Scratch;
    llvm::raw_svector_ostream OS(Scratch);
    auto Protocol = Factory.makeProtocolKeyword("", " ");
    auto MyCollection = Factory.makeIdentifier("MyCollection", "", "");
    auto ElementName = Factory.makeIdentifier("Element", "", "");
    auto ElementParam = Factory.makePrimaryAssociatedType(
        /*UnexpectedNodes=*/None, ElementName, /*UnexpectedNodes=*/None, None);
    auto LeftAngle = Factory.makeLeftAngleToken("", "");
    auto RightAngle = Factory.makeRightAngleToken("", " ");
    auto PrimaryAssocs = PrimaryAssociatedTypeClauseSyntaxBuilder(Arena)
                             .useLeftAngleBracket(LeftAngle)
                             .useRightAngleBracket(RightAngle)
                             .addPrimaryAssociatedType(ElementParam)
                             .build();

    auto LeftBrace = Factory.makeLeftBraceToken("", "");
    auto RightBrace = Factory.makeRightBraceToken("", "");
    auto Members = MemberDeclBlockSyntaxBuilder(Arena)
                      .useLeftBrace(LeftBrace)
                      .useRightBrace(RightBrace)
                      .build();
    Factory
        .makeProtocolDecl(/*UnexpectedNodes=*/None, None, /*UnexpectedNodes=*/None,
                          None, /*UnexpectedNodes=*/None, Protocol,
                          /*UnexpectedNodes=*/None, MyCollection,
                          /*UnexpectedNodes=*/None, PrimaryAssocs,
                          /*UnexpectedNodes=*/None, None, /*UnexpectedNodes=*/None,
                          None, /*UnexpectedNodes=*/None, Members)
        .print(OS);
    ASSERT_EQ(OS.str().str(),
              "protocol MyCollection<Element> {}");
  }
}
