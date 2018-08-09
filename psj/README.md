## Welcome to preciouStar's Page


### Project Introduction
```
비즈니스 로직에서 디자인 패턴을 커스터마이징할 때 DAO나 서비스단에서 템플릿 메소드 패턴을 근거하여 
상위 인터페이스에 상속받은 하위 클래스에 상세부분에 관한 코딩을 하였고, Value Object(VO) 같은 경우에는 
리팩토링에 유리한 스트래티지 패턴에 근거하여 DB 필드값을 private로 캡슐화하여 데이터들을 보호한다. 
Log4j의 upgrade된  버전의 xml같은 환경설정에 변화가 있더라도 바로 동기화하여 WAS에 반영하고, 
속도도 빠른 Logback을 사용하였고, 뷰상에서는 회원가입시 form에 요청을 날릴때 input에 히든을 부여해서
페이지 이동시, 동적으로 변하는 파라미터인 1회성 인증키인 csrf토큰을 추가 하여 CSRF 공격을 방어
스프링 시큐리티에서 프록시 기반의 필터가 인터셉트하여 사용자 권한에 따른 게시판 CRUD 커스터마이징
```


### Development Environment
기술 | 세부 사항
------------------------ | -------------------------
FRONT-END| HTML5, CSS3, JavaScript, Ajax(JSON), REST API, jQuery v1.11.2, Bootstrap v3.3.2, jqGrid
BACK-END | Java 8, JSP, Spring Framework v4.3.12, Spring Security v3.2.5, MyBatis v3.4.4
DATABASE | MySQL v5.7.21

