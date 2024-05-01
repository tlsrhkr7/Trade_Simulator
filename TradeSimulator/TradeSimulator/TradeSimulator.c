#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Sleep 함수를 사용하기 위한 헤더 파일

// 랜덤 가격 변동 범위 정의
#define PRICE_CHANGE_MIN -100.0 // 코인 가격의 최소 변동 범위
#define PRICE_CHANGE_MAX 100.0   // 코인 가격의 최대 변동 범위

// 랜덤 가격 변동 함수
double generatePriceChange(double currentPrice) {
    // 랜덤 가격 변동 생성
    double change = ((double)rand() / (double)RAND_MAX) * (PRICE_CHANGE_MAX - PRICE_CHANGE_MIN) + PRICE_CHANGE_MIN;

    // 현재 가격에 변동 더하기
    return currentPrice + change;
}

// 색상 지정 매크로
#define RED     12
#define BLUE    9
#define YELLOW  14
#define DEFAULT 7

// 코인 정보 구조체
typedef struct {
    char* name;          // 코인 이름
    double initialPrice; // 초기 가격
    double prevPrice;    // 이전 가격
    double quantity;     // 보유량
    double totalCost;    // 누적 매수 금액
    double profit;       // 수익금
} Coin;

// 콘솔 텍스트 색상 변경 함수
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

int main() {
    // 시드를 현재 시간으로 초기화
    srand((unsigned int)time(NULL));

    // 초기 자산 입력
    double totalAssets;
    printf("초기 자산을 입력하세요: ");
    scanf("%lf", &totalAssets);

    // 코인 개수
    int numCoins = 5;

    // 코인 포트폴리오 동적 할당
    Coin* portfolio = (Coin*)malloc(numCoins * sizeof(Coin));

    // 코인 초기화
    char* coinNames[] = { "SDJ", "LSO", "PDH", "ADH", "JIH" };
    double coinInitialPrices[] = { 1000, 1000, 1000, 1000, 1000 };
    for (int i = 0; i < numCoins; ++i) {
        portfolio[i].name = coinNames[i];
        portfolio[i].initialPrice = coinInitialPrices[i];
        portfolio[i].prevPrice = coinInitialPrices[i];
        portfolio[i].quantity = 0;
        portfolio[i].totalCost = 0;
        portfolio[i].profit = 0;
    }

    double cash = totalAssets; // 초기 현금 설정

    // 반복해서 코인 정보 출력
    while (1) {
        // 화면 지우기
        system("cls");

        // 각 코인의 가격 변동 적용
        for (int i = 0; i < numCoins; ++i) {
            portfolio[i].prevPrice = portfolio[i].initialPrice; // 이전 가격 저장
            portfolio[i].initialPrice = generatePriceChange(portfolio[i].initialPrice);
            // 가격이 0이 되면 상장폐지
            if (portfolio[i].initialPrice <= 0.000001) {
                printf("%s 코인이 상장폐지되었습니다.\n", portfolio[i].name);
                // 코인 리스트에서 제거
                for (int j = i; j < numCoins - 1; ++j) {
                    portfolio[j] = portfolio[j + 1];
                }
                numCoins--; // 코인 개수 감소
                i--; // 배열 인덱스 보정
            }
        }

        // 총 자산 및 잔액 출력
        double balance = cash;
        for (int i = 0; i < numCoins; ++i) {
            balance += portfolio[i].quantity * portfolio[i].initialPrice;
        }
        printf("총 자산: $%f\n", balance);
        printf("남은 현금: $%f\n", cash);

        // 코인 정보 출력
        printf("-----------------------------------------------------------------\n");
        printf(" %-8s | %-12s | %-20s | %-20s \n", "코인", "현재 가격", "전일대비(%)", "전일대비(가격)");
        printf("-----------------------------------------------------------------\n");
        for (int i = 0; i < numCoins; ++i) {
            double changePercentage = (portfolio[i].initialPrice - portfolio[i].prevPrice) / portfolio[i].prevPrice * 100.0;
            double changePrice = portfolio[i].initialPrice - portfolio[i].prevPrice;
            if (changePercentage > 0) {
                setColor(RED); // 양수일 경우 빨간색
            }
            else if (changePercentage < 0) {
                setColor(BLUE); // 음수일 경우 파란색
            }
            else {
                setColor(DEFAULT); // 0일 경우 기본색
            }
            printf(" %-8s | $%-12.6f | %+12.6f%% | %+12.6f         \n", portfolio[i].name, portfolio[i].initialPrice, changePercentage, changePrice);
            setColor(DEFAULT); // 기본색으로 설정
        }
        printf("-----------------------------------------------------------------\n");
        // 포트폴리오 출력
        printf("\n포트폴리오:\n");
        for (int i = 0; i < numCoins; ++i) {
            if (portfolio[i].quantity > 0) {
                double avgPrice = portfolio[i].totalCost / portfolio[i].quantity;
                double profitPercentage = (portfolio[i].initialPrice - avgPrice) / avgPrice * 100.0;
                double currentProfit = portfolio[i].quantity * (portfolio[i].initialPrice - avgPrice);
                portfolio[i].profit = currentProfit;
                setColor(YELLOW); // 기본색으로 설정
                printf("%s: 보유량 : %.6f개, 평균매수가 : $%.6f, 현재가 : $%.6f ", portfolio[i].name, portfolio[i].quantity, avgPrice, portfolio[i].initialPrice);
                if (profitPercentage > 0) {
                    setColor(RED); // 양수일 경우 빨간색
                }
                else if (profitPercentage < 0) {
                    setColor(BLUE); // 음수일 경우 파란색
                }
                printf("수익률 : %.2f % %, 수익금 : $%.6f\n", profitPercentage, currentProfit);
                setColor(DEFAULT);
            }
        }

        // 옵션 선택
        printf("\n1. 매수\n");
        printf("2. 매도\n");
        printf("3. 종료\n");
        printf("선택: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            // 매수 기능 추가
            printf("\n매수할 코인을 선택하세요:\n");
            for (int i = 0; i < numCoins; ++i) {
                printf("%d. %s\n", i + 1, portfolio[i].name);
            }
            printf("선택: ");
            int buyChoice;
            scanf("%d", &buyChoice);
            if (buyChoice >= 1 && buyChoice <= numCoins) {
                printf("\n매수할 %s 코인의 양을 입력하세요: ", portfolio[buyChoice - 1].name);
                double quantity;
                scanf("%lf", &quantity);
                if (quantity * portfolio[buyChoice - 1].initialPrice <= cash) {
                    cash -= quantity * portfolio[buyChoice - 1].initialPrice;
                    portfolio[buyChoice - 1].quantity += quantity;
                    portfolio[buyChoice - 1].totalCost += quantity * portfolio[buyChoice - 1].initialPrice;
                    printf("\n%.6f %s 코인을 $%.2f에 매수하였습니다.\n", quantity, portfolio[buyChoice - 1].name, quantity * portfolio[buyChoice - 1].initialPrice);
                }
                else {
                    printf("현금이 부족하여 매수할 수 없습니다.\n");
                }
            }
            else {
                printf("잘못된 선택입니다.\n");
            }
            break;
        case 2:
            // 매도 기능 추가
            printf("\n매도할 코인을 선택하세요:\n");
            for (int i = 0; i < numCoins; ++i) {
                printf("%d. %s\n", i + 1, portfolio[i].name);
            }
            printf("선택: ");
            int sellChoice;
            scanf("%d", &sellChoice);
            if (sellChoice >= 1 && sellChoice <= numCoins) {
                printf("매도할 %s 코인의 양을 입력하세요: ", portfolio[sellChoice - 1].name);
                double quantity;
                scanf("%lf", &quantity);
                if (quantity <= portfolio[sellChoice - 1].quantity) {
                    cash += quantity * portfolio[sellChoice - 1].initialPrice;
                    portfolio[sellChoice - 1].quantity -= quantity;
                    printf("%.6f %s 코인을 $%.2f에 매도하였습니다.\n", quantity, portfolio[sellChoice - 1].name, quantity * portfolio[sellChoice - 1].initialPrice);
                }
                else {
                    printf("보유한 코인 양보다 많이 매도할 수 없습니다.\n");
                }
            }
            else {
                printf("잘못된 선택입니다.\n");
            }
            break;
        case 3:
            printf("프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("잘못된 선택입니다. 다시 선택해주세요.\n");
            break;
        }

        // 1초 대기
        Sleep(1000); // 1초 대기
    }

    // 포트폴리오 동적 할당 해제
    free(portfolio);

    return 0;
}