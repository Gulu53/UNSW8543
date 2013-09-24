--User Defined Types
create type Stock_Daily as ("Date" date, Code char(3), Volume integer, Preprice numeric, Price numeric, Change numeric, Gain numeric);
create type AXE_Record as ("Date" date, Code char(3), Volume integer);
create type Sector_Avg as (Sector text, AvgRating numeric);
create type Relax_Company as (Code char(3), Name text, Address text, Zip varchar(10), Sector text);
create type Stock_Overall_Gain as (Code char(3), BeginPrice numeric, EndPrice numeric, Change numeric, Gain numeric);
create type Stock_Info as (Code char(3), MinPrice numeric, AvgPrice numeric, MaxPrice numeric, MinDayGain numeric, AvgDayGain numeric, MaxDayGain numeric);
--END User Defined Types

--Views
create or replace view Q1(Name, Country) as select Name, Country from Company where country <> 'Australia';

create or replace view Q2(Code) as select code from (select code, count(*) from executive group by code having count(*) > 5) as a;

create or replace view Q3(Name) as select name from Company where code in (select code from Category where sector = 'Technology');

create or replace view Q4(Sector, Number) as select sector, count(*) as number from Category group by sector;

create or replace view Q5(Name) as select distinct person from executive where code in (select code from company where name in (select * from Q3));

create or replace view Q6(Name) as select name from company where code in (select code from category where sector = 'Services') and country = 'Australia' and zip like '2%';

create or replace function stock_daily_breakdown() returns setof Stock_Daily
as $$
--Bring the price of the next day back to the same row with today's price named prevprice using Lag function.  
select "Date", code, volume, prevprice, price, (price - prevprice) as Change, (((price - prevprice) / prevprice) * 100) as Gain 
from (select *, lag(price) over (partition by code order by "Date") as prevprice from ASX) as a 
where a.prevprice is not null;
$$ language sql;
create or replace view Q7("Date", Code, Volume, PrevPrice, Price, Change, Gain) as select * from stock_daily_breakdown();

create or replace function active_stock() returns setof AXE_Record
as $$
declare
	r record;
begin
	-- For each trading day, find out the value of maximum trading volume. 
	for r in select "Date" as d, max(volume) as maxvol from ASX group by "Date" order by "Date"
	loop
		-- returns all the company has the maximum trading volume on a particular day. 
		return query (select "Date", Code, Volume from ASX where "Date" = r.d and volume = r.maxvol); 
	end loop;
end;
$$ language plpgsql;
create or replace view Q8("Date", Code, Volume) as select * from active_stock() order by "Date", code;

create or replace view Q9(Sector, Industry, Number) as select sector, industry, count(*) from Category group by industry, sector order by sector, industry;

create or replace view Q10(Code, Industry) as select code, industry from Category where industry in (select industry from Q9 where number = 1);

create or replace function sector_score() returns setof Sector_Avg
as $$
declare
	r record;
	ans Sector_Avg;
begin
	for r in select distinct sector from category
	loop
		ans.Sector = r.sector;
		--For all the companies in a particular sector, return their avearge rating
		ans.AvgRating := (select avg(Star) from rating where code in (select code from category where sector = r.sector));
		return next ans; 
	end loop;
end;
$$ language plpgsql;
create or replace view Q11(Sector, AvgRating) as select * from sector_score() order by AvgRating desc;

create or replace view Q12(Name) as select person from (select person, count(*) from executive group by person having count(*) > 1) as a;

create or replace function competitor_free_company() returns setof Relax_Company
as $$
declare
	r record;
	t record;
	aus_comp_flag integer := 0;
	oversea_comp_flag integer := 0;
	ans Relax_Company;
begin
	--For each distinct sector in the Category table
	for r in select distinct sector from category
	loop
		--For each company in a particular sector
		for t in select code from category where sector = r.sector
		loop
			--If there is an oversea company exist in a particular sector
			if ((select country from Company where code = t.code) <> 'Australia') then
				--Set the flag and break from current iteration
				oversea_comp_flag := 1;
				EXIT;
			else
			--Otherwise set Australia company flag to 1.
				aus_comp_flag := 1;
			end if;	 
		end loop;
		--If there is no onversea company in a particular sector
		if (oversea_comp_flag = 0) then
			--And if there is Australia conpany exists in this sector
			if (aus_comp_flag = 1) then
				--recycle the flag for next interation
				aus_comp_flag := 0;
				--Select every single company in this sector to output.
				for t in select code from category where sector = r.sector
				loop
					ans.Code = t.code;
					ans.Name = (select name from company where code = t.code);
					ans.Address = (select address from company where code = t.code);
					ans.Zip = (select zip from company where code = t.code);
					ans.Sector = r.sector;
					return next ans; 
				end loop;
			end if;
		else
		--Otherwise recycle the flags and move on to next sector.
			aus_comp_flag = 0;
			oversea_comp_flag = 0;
		end if; 
	end loop;
end;
$$ language plpgsql;
create or replace view Q13(Code, Name, Address, Zip, Sector) as select * from competitor_free_company();

create or replace function stock_gain_summary() returns setof Stock_Overall_Gain
as $$ 
declare 
	r record;
	ans Stock_Overall_Gain;
begin
	--For each company in the asx table
	for r in select distinct code from asx
	loop
		ans.Code = r.code;
		ans.BeginPrice = (select price from asx where "Date" = (select min("Date") from asx where code = r.code) and code = r.code);
		ans.EndPrice = (select price from asx where "Date" = (select max("Date") from asx where code = r.code) and code = r.code);
		ans.Change = ans.EndPrice - ans.BeginPrice;
		ans.Gain = ((ans.Change / ans.BeginPrice) * 100);
		return next ans;
	end loop;
end;
$$ language plpgsql;
create or replace view Q14(Code, BeginPrice, EndPrice, Change, Gain) as select * from stock_gain_summary() order by gain desc, code asc;

create or replace function stock_info_summary() returns setof Stock_Info
as $$
--Inner join the results from two queries on the company code.  
select a.code, a.MinPrice, a.AvgPrice, a.MaxPrice, b.MinDayGain, b.AvgDayGain, b.MaxDayGain 
from (select code, min(price) as MinPrice, avg(price) as AvgPrice, max(price) as MaxPrice from asx group by code order by code) as a 
join (select code, min(gain) as MinDayGain, avg(gain) as AvgDayGain, max(gain) as MaxDayGain from Q7 group by code order by code) as b 
on a.code = b.code;
$$ language sql;
create or replace view Q15(Code, MinPrice, AvgPrice, MaxPrice, MinDayGain, AvgDayGain, MaxDayGain) as select * from stock_info_summary();
--END Views

--Triggers
create or replace function check_executive_func() returns trigger
as $$
begin
	--If this particular person already exists in the executive table.
	if ((select count(person) from executive where person = new.person) <> 0) then
		raise exception 'Person cannot be an executive of more than one company.';
	else
	--Otherwise return the new value for insert or update.
		return new;
    end if;
end;
$$ language plpgsql;
create trigger check_executive before insert or update on Executive for each row execute procedure check_executive_func();

create or replace function update_stock_rating() returns trigger
as $$
declare
	this_sector text;
begin
	--Retrieve the sector where this particular company is belongs to
	this_sector = (select sector from Category where code = new.code);
	update Rating
	set Star = 5
	--where the code equals to the companies have the max gains on a particular date in its sector
	where Code in (select code from Q7 where gain = (select max(gain) from Q7 where code in (select code from Category where sector = this_sector) and "Date" = new."Date") and "Date" = new."Date");
	update Rating
	set Star = 1
	--where the code equals to the companies have the min gains on a particular date in its sector
	where Code in (select code from Q7 where gain = (select min(gain) from Q7 where code in (select code from Category where sector = this_sector) and "Date" = new."Date") and "Date" = new."Date");
	return null; 
end;
$$ language plpgsql;
create trigger update_rating after insert on ASX for each row execute procedure update_stock_rating();

create or replace function logging_stock_update() returns trigger
as $$
begin
	--Insert current time as data type of timestamp and old data into log table
	insert into ASXLog values (
		now(),
		old."Date",
		old.code,
		old.volume,
		old.price
	);	
	return null;
end;
$$ language plpgsql;
create trigger logging_asx_update after update on ASX for each row execute procedure logging_stock_update();
--END Triggers

