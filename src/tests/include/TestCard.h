#pragma once
#include <QObject>

class TestCard : public QObject {
    Q_OBJECT
private slots:
    // Constructor tests
    void testDefaultConstructor();
    void testParameterizedConstructor();
    void testCopyConstructor();
    void testCopyAssignmentOperator();
    void testMoveConstructor();
    void testMoveAssignmentOperator();
    void testMoveFromTemporary();
    void testMoveSelfAssignment();
    void testMoveEmptyObject();
    void testMoveAfterCopy();
    void testMovePerformance();
    void testMoveThenUseSource();

    // Setter tests
    void testSetId();
    void testSetQuestion();
    void testSetAnswer();
    void testSetContentType();
    void testSetTestMode();
    void testSetEasyFactor();
    void testSetIntervalDays();
    void testSetRepetitions();
    void testSetNextReview();
    void testSetLastReview();
    void testSetDeckId();

    // Getter tests
    void testGetId();
    void testGetQuestion();
    void testGetAnswer();
    void testGetContentType();
    void testGetTestMode();
    void testGetEasyFactor();
    void testGetIntervalDays();
    void testGetRepetitions();
    void testGetNextReview();
    void testGetLastReview();
    void testGetDeckId();

    // Integration tests
    void testSetAndGetConsistency();
    void testEdgeCases();
};
