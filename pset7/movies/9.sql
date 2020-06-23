-- In 9.sql, write a SQL query to list the names of all people who starred
-- in a movie released in 2004, ordered by birth year.
SELECT people.name FROM people
WHERE people.id IN
(SELECT stars.person_id FROM stars
JOIN movies on stars.movie_id = movies.id
WHERE movies.year = 2004)
ORDER BY people.birth