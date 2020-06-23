-- In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.

SELECT DISTINCT(people.name) FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON movies.id = stars.movie_id
WHERE stars.movie_id IN
(SELECT movies.id FROM movies
JOIN stars ON stars.movie_id = movies.id
WHERE stars.person_id IN
(SELECT people.id FROM people
WHERE people.name = "Kevin Bacon"
)) AND people.name != "Kevin Bacon"
ORDER BY people.name