--User created type
create type AXE_Record as ("Date" date, Code char(3), Volume integer);
create type Sector_Avg as (Sector text, AvgRating numeric);

--User defined functions
create or replace function active_stock() returns setof AXE_Record
as $$
declare
	r record;
begin
	for r in select "Date" as d, max(volume) as maxvol from ASX group by "Date" order by "Date"
	loop
		 return query (select "Date", Code, Volume from ASX where "Date" = r.d and volume = r.maxvol); 
	end loop;
end;
$$ language plpgsql;

create or replace function sector_score() returns setof Sector_Avg
as $$
declare
	r record;
	ans Sector_Avg;
begin
	for r in select distinct sector from category
	loop
		ans.Sector = r.sector;
		ans.AvgRating := (select cast(avg(Star) as decimal(10,2)) from rating where code in (select code from category where sector = r.sector));
		return next ans; 
	end loop;
end;
$$ language plpgsql;

--Views
create or replace view Q1(Name, Country) as select Name, Country from Company where country <> 'Australia';
create or replace view Q2(Code) as select code from (select code, count(*) from executive group by code having count(*) > 5) as a;
create or replace view Q3(Name) as select name from Company where code in (select code from Category where sector = 'Technology');
create or replace view Q4(Sector, Number) as select sector, count(*) as number from Category group by sector;
create or replace view Q5(Name) as select distinct person from executive where code in (select code from company where name in (select * from Q3));
create or replace view Q6(Name) as select name from company where code in (select code from category where sector = 'Services') and country = 'Australia' and zip like '2%';
create or replace view Q7("Date", Code, Volume, PrevPrice, Price, Change, Gain) as select "Date", code, volume, prevprice, price, (price - prevprice) as Change, cast((((price - prevprice) / prevprice) * 100) as decimal(10,2)) as Gain from (select *, lag(price) over (partition by code order by "Date") as prevprice from ASX) as a where a.prevprice is not null;
create or replace view Q8("Date", Code, Volume) as select * from active_stock() order by "Date", code;
create or replace view Q9(Sector, Industry, Number) as select sector, industry, count(*) from Category group by industry, sector order by sector, industry;
create or replace view Q10(Code, Industry) as select code, industry from Category where industry in (select industry from Q9 where number = 1);
create or replace view Q11(Sector, AvgRating) as select * from sector_score();
