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

    // SM2 algorithm tests
    void testUpdateSM2FirstReview();
    void testUpdateSM2ExcellentGrade();
    void testUpdateSM2GoodGrade();
    void testUpdateSM2MediumGrade();
    void testUpdateSM2PoorGrade();
    void testUpdateSM2FailGrade();
    void testUpdateSM2GradeBoundaries();
    void testUpdateSM2MultipleReviews();
    void testUpdateSM2AfterFail();
    void testUpdateSM2EasyFactorBounds();
    void testUpdateSM2ScheduleProgression();
};
