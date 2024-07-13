#define _CRT_SECURE_NO_WARNINGS
#include <mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    MYSQL *conn1;
    MYSQL_RES *res;
    MYSQL_ROW row;

    char *server = "localhost";
    char *user = "root";
    char *password = "autoset";
    char *database = "edb_PROJECT";
    char query[1000];
    char from[10];
    char to[73][10] = {0};
    int min = 1000;
    int real_to = 0;
    char category[15] = "";
    int flag = -1;
    int k = 0;

    printf("분류 또는 진료과목: ");
    scanf("%s", category);

    if (!strcmp(category, "가정의학과") || !strcmp(category, "내과") || !strcmp(category, "마취통증의학과") || !strcmp(category, "비뇨의학과") || !strcmp(category, "산부인과") || !strcmp(category, "소아청소년과") || !strcmp(category, "신경외과") || !strcmp(category, "외과") || !strcmp(category, "이비인후과") || !strcmp(category, "정신건강의학과") || !strcmp(category, "정형외과") || !strcmp(category, "종합") || !strcmp(category, "치과") || !strcmp(category, "피부과") || !strcmp(category, "한의원"))
        flag = 1;
    else if (!strcmp(category, "은행"))
        flag = 2;
    else if (!strcmp(category, "복지센터"))
        flag = 3;
    else if (!strcmp(category, "고기") || !strcmp(category, "마트") || !strcmp(category, "베트남") || !strcmp(category, "분식") || !strcmp(category, "술집") || !strcmp(category, "양식") || !strcmp(category, "일식") || !strcmp(category, "중식") || !strcmp(category, "치킨") || !strcmp(category, "카페") || !strcmp(category, "패스트푸드") || !strcmp(category, "편의점") || !strcmp(category, "피자") || !strcmp(category, "한식"))
        flag = 4;
    else
    {
        printf("wrong category");
        exit(1);
    }

    printf("출발지: ");
    scanf("%s", from);

    conn1 = mysql_init(NULL); // conn1에 db 연결
    if (!mysql_real_connect(conn1, server, user, password, database, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn1));
        exit(1);
    }

    mysql_set_character_set(conn1, "euckr");
    mysql_query(conn1, "set names euckr");

    if (flag == 1)
    {
        sprintf(query, "select 횡단보도관리번호 from 병원;");
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류1: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            strcpy(to[k], row[0]);
            k++;
        }
    }

    else if (flag == 2)
    {
        sprintf(query, "select 횡단보도관리번호 from 은행;");
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류1: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            strcpy(to[k], row[0]);
            k++;
        }
    }
    else if (flag == 3)
    {
        sprintf(query, "select 횡단보도관리번호 from 복지센터;");
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류1: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            strcpy(to[k], row[0]);
            k++;
        }
    }

    else if (flag == 4)
    {
        sprintf(query, "select 횡단보도관리번호 from 음식점 where 분류 = '%s';", category);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            strcpy(to[k], row[0]);
            k++;
        }

        mysql_free_result(res);
    }

    for (int i = 0; i < k; i++)
    {
        sprintf(query, "select 소요시간 from 도보경로 where 출발지 = '%s' and 도착지 = '%s';", from, to[i]);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "index: %d, 쿼리 실행 오류4: %s\n", i, mysql_error(conn1));
            exit(1);
        }

        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류1: %s\n", mysql_error(conn1));
            exit(1);
        }

        while ((row = mysql_fetch_row(res)) != NULL)
        {
            if (min > atoi(row[0]))
            {
                min = atoi(row[0]);
                real_to = i;
            }
        }
        mysql_free_result(res);
    }

    printf("최적 도착지: %s\n", to[real_to]);
    printf("도보경로: ");
    sprintf(query, "select 도보경로 from 도보경로 where 출발지 = '%s' and 도착지 = '%s';", from, to[real_to]);
    if (mysql_query(conn1, query))
    {
        fprintf(stderr, "쿼리 실행 오류5: %s\n", mysql_error(conn1));
        exit(1);
    }
    res = mysql_store_result(conn1);
    if (!res)
    {
        fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
        exit(1);
    }
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s", row[0]);
    }
    mysql_free_result(res);

    printf("\n소요시간: ");
    sprintf(query, "select 소요시간 from 도보경로 where 출발지 = '%s' and 도착지 = '%s';", from, to[real_to]);
    if (mysql_query(conn1, query))
    {
        fprintf(stderr, "쿼리 실행 오류6: %s\n", mysql_error(conn1));
        exit(1);
    }
    res = mysql_store_result(conn1);
    if (!res)
    {
        fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
        exit(1);
    }
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("%s", row[0]);
    }
    printf("분");

    mysql_free_result(res);
    if (flag == 1)
    {
        k = 1;
        sprintf(query, "select 병원명 from 병원 where 횡단보도관리번호 = '%s' and 진료과목 = '%s';", to[real_to], category);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류7: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            printf("\n병원 이름 %d: %s", k + 1, row[0]);
            k++;
        }
    }

    else if (flag == 2)
    {
        k = 1;
        sprintf(query, "select 은행지점명 from 은행 where 횡단보도관리번호 = '%s';", to[real_to]);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류8: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            printf("\n은행 이름 %d: %s", k + 1, row[0]);
            k++;
        }
    }
    else if (flag == 3)
    {
        k = 1;
        sprintf(query, "select 복지센터 from 복지센터명 where 횡단보도관리번호 = '%s';", to[real_to]);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류9: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {
            printf("\n복지센터 이름 %d: %s", k + 1, row[0]);
            k++;
        }
    }
    else if (flag == 4)
    {
        k = 1;
        sprintf(query, "select 사업장명 from 음식점 where 횡단보도관리번호 = '%s' and 분류 = '%s';", to[real_to], category);
        if (mysql_query(conn1, query))
        {
            fprintf(stderr, "쿼리 실행 오류10: %s\n", mysql_error(conn1));
            exit(1);
        }
        res = mysql_store_result(conn1);
        if (!res)
        {
            fprintf(stderr, "결과 가져오기 오류2: %s\n", mysql_error(conn1));
            exit(1);
        }
        while ((row = mysql_fetch_row(res)) != NULL)
        {

            printf("\n식당 이름 %d: %s", k, row[0]);
            k++;
        }
    }
    else
        printf("flag failed");
    mysql_free_result(res);
    mysql_close(conn1);

    return 0;
}
