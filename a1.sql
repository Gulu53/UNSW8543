-- COMP9312 Assignment 1
-- Schema for eAuction
-- Written by NilH 
-- Student ID: 3477613 

-- DOMAINS
create domain NameType as varchar(100);

create domain IDType as integer check (value >= 1 and  value <= 2147483647);

create domain EmailType as varchar(100) check (value like '%@%.%');

create domain PhoneType as char(10) check (value ~ '[0-9]{10}');

create domain PostcodeType as char(4) check (value ~ '[0-9]{4}');

create domain PasswordType as varchar(10) check (value ~ '^[a-zA-Z0-9]+$');

create domain RatingType as integer check (value >= 1 and value <= 5); 

create domain BankNameType as varchar(100);

create domain BankAccountType as varchar(10) check (value ~ '^[0-9]+$'); 

create domain MoneyType as numeric check (value > 0);

-- TABLES
create table Member (
    name        NameType not null, 
    member_id   serial,
    password    PasswordType not null,
    email       EmailType not null,
    address     text not null,
    postcode    PostcodeType not null,
    phone       PhoneType,
    primary key (member_id)
);

create table Buyer (
    buyer_id        IDType, 
    ship_address    text not null,   
    ship_postcode   PostcodeType not null,
    foreign key (buyer_id) references Member(member_id),
    primary key (buyer_id)
);

create table Seller (
    seller_id           IDType, 
    bank_name           BankNameType not null, 
    account             BankAccountType not null,
    --The total participation of seller in seller to item relationship is not 
    --captured here at schema level. The constraint needs to check the value from Item table
    --this practice will be expensive in terms of computation time.
    --This constraint however can be captured by a before insert trigger.  
    foreign key (seller_id) references Member(member_id),
    primary key (seller_id) 
);

create table Item (
    item_id         serial, 
    seller_id       IDType,     --not total participation, to allow member to sell first item and becomes a seller         
    title           text not null,
    description     text not null,
    price           MoneyType not null,
    "increment"     MoneyType not null,
    start_date      timestamp with time zone not null, 
    end_date        timestamp with time zone not null,
    foreign key (seller_id) references Seller(seller_id),
    primary key (item_id)
);

create table Accessory (
    parent_item_id  IDType,
    accessory_id    integer not null check (accessory_id >= 1 and accessory_id <= 9),
    title           text not null,
    description     text not null,
    foreign key (parent_item_id) references Item(item_id),
    primary key (parent_item_id, accessory_id)
);

create table Category (
    name                    text,
    parent_category_name    text default null,  
    foreign key (parent_category_name) references Category(name),
    primary key (name)
);d

create table Transaction (
    transaction_id  serial,
    buyer_id        IDType not null,
    seller_id       IDType not null,
    item_id         IDType unique not null, --total participation and 1:1 relationship
    commission      MoneyType not null,
    shipping_cost   MoneyType not null,
    rating          RatingType,
    "comment"       text,
    foreign key (item_id) references Item(item_id),
    foreign key (buyer_id) references Buyer(buyer_id),
    foreign key (seller_id) references Seller(seller_id),
    primary key (transaction_id)
);

create table Bid (
    buyer_id            IDType,
    item_id             IDType not null,  
    transaction_id      IDType,
    price               MoneyType not null,
    "when"              timestamp with time zone,  
    foreign key (buyer_id) references Buyer(buyer_id),
    foreign key (item_id) references Item(item_id),
    foreign key (transaction_id) references Transaction(transaction_id), 
    primary key (buyer_id, "when", item_id)    --This constraint assumes that the same buyer cannot creates two bids at the same time on the same item. 
);

create table ItemBelongsToCategory (
    item_id         IDType,
    category_name   text,
    foreign key (item_id) references Item(item_id),
    foreign key (category_name) references Category(name),
    primary key (item_id, category_name)
);

