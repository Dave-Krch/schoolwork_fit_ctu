create or replace function find_possible_crimes()
returns table(date_commited date, description varchar(64))
language plpgsql
as $function$
declare
    crime crimes%ROWTYPE;
    out crimes[];

    alibi_count int;
begin
    out := ARRAY[]::crimes[];

    FOR crime in SELECT * FROM crimes LOOP
        select count(*)
        from suspect_activities s_activities
            join (trusted_people_activities ta
                join trusted_people tp
                on ta.id = tp.id) t_activities
            on s_activities.name_with = t_activities.name and s_activities.place = t_activities.place
        where crime.date_commited >= s_activities.from_date
            and crime.date_commited <= s_activities.to_date
            and crime.date_commited >= t_activities.from_date
            and crime.date_commited <= t_activities.to_date
        into alibi_count;

        RAISE NOTICE 'Alibi count for day % is %', crime.date_commited, alibi_count;

        if alibi_count != 1 then out := array_append(out, crime);
        end if;

    end loop;

    return query select * from unnest(out);

end;$function$