DROP SCHEMA public CASCADE;
CREATE SCHEMA public;

-- This needs  PostgreSQL version >= 9.1 
CREATE EXTENSION dblink;

\i tables.sql
\i helper_procedures.sql
\i triggers.sql
\i initial_data.sql
