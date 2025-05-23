PGDMP  +                    }           Chat    17.2    17.2 ;               0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false                       0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false                       0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false                        1262    24972    Chat    DATABASE     y   CREATE DATABASE "Chat" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Polish_Poland.1252';
    DROP DATABASE "Chat";
                     postgres    false            �            1259    25001 	   user_chat    TABLE     �   CREATE TABLE public.user_chat (
    log_id integer NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
)
PARTITION BY RANGE (send_date);
    DROP TABLE public.user_chat;
       public         p       postgres    false            �            1259    25000    user_chat_log_id_seq    SEQUENCE     �   CREATE SEQUENCE public.user_chat_log_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 +   DROP SEQUENCE public.user_chat_log_id_seq;
       public               postgres    false    220            !           0    0    user_chat_log_id_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.user_chat_log_id_seq OWNED BY public.user_chat.log_id;
          public               postgres    false    219            �            1259    25018    user_chat_y2025m05    TABLE       CREATE TABLE public.user_chat_y2025m05 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m05;
       public         heap r       postgres    false    219    220            �            1259    25033    user_chat_y2025m06    TABLE       CREATE TABLE public.user_chat_y2025m06 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m06;
       public         heap r       postgres    false    219    220            �            1259    25048    user_chat_y2025m07    TABLE       CREATE TABLE public.user_chat_y2025m07 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m07;
       public         heap r       postgres    false    219    220            �            1259    25063    user_chat_y2025m08    TABLE       CREATE TABLE public.user_chat_y2025m08 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m08;
       public         heap r       postgres    false    219    220            �            1259    25078    user_chat_y2025m09    TABLE       CREATE TABLE public.user_chat_y2025m09 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m09;
       public         heap r       postgres    false    219    220            �            1259    25093    user_chat_y2025m10    TABLE       CREATE TABLE public.user_chat_y2025m10 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m10;
       public         heap r       postgres    false    219    220            �            1259    25108    user_chat_y2025m11    TABLE       CREATE TABLE public.user_chat_y2025m11 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m11;
       public         heap r       postgres    false    219    220            �            1259    25123    user_chat_y2025m12    TABLE       CREATE TABLE public.user_chat_y2025m12 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL
);
 &   DROP TABLE public.user_chat_y2025m12;
       public         heap r       postgres    false    219    220            �            1259    24974    users    TABLE     �   CREATE TABLE public.users (
    user_id integer NOT NULL,
    user_name character varying(64) NOT NULL,
    user_name_id integer NOT NULL,
    password text NOT NULL
);
    DROP TABLE public.users;
       public         heap r       postgres    false            �            1259    24973    users_user_id_seq    SEQUENCE     �   CREATE SEQUENCE public.users_user_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 (   DROP SEQUENCE public.users_user_id_seq;
       public               postgres    false    218            "           0    0    users_user_id_seq    SEQUENCE OWNED BY     G   ALTER SEQUENCE public.users_user_id_seq OWNED BY public.users.user_id;
          public               postgres    false    217            F           0    0    user_chat_y2025m05    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m05 FOR VALUES FROM ('2025-05-01 00:00:00+00') TO ('2025-06-01 00:00:00+00');
          public               postgres    false    221    220            G           0    0    user_chat_y2025m06    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m06 FOR VALUES FROM ('2025-06-01 00:00:00+00') TO ('2025-07-01 00:00:00+00');
          public               postgres    false    222    220            H           0    0    user_chat_y2025m07    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m07 FOR VALUES FROM ('2025-07-01 00:00:00+00') TO ('2025-08-01 00:00:00+00');
          public               postgres    false    223    220            I           0    0    user_chat_y2025m08    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m08 FOR VALUES FROM ('2025-08-01 00:00:00+00') TO ('2025-09-01 00:00:00+00');
          public               postgres    false    224    220            J           0    0    user_chat_y2025m09    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m09 FOR VALUES FROM ('2025-09-01 00:00:00+00') TO ('2025-10-01 00:00:00+00');
          public               postgres    false    225    220            K           0    0    user_chat_y2025m10    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m10 FOR VALUES FROM ('2025-10-01 00:00:00+00') TO ('2025-11-01 00:00:00+00');
          public               postgres    false    226    220            L           0    0    user_chat_y2025m11    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m11 FOR VALUES FROM ('2025-11-01 00:00:00+00') TO ('2025-12-01 00:00:00+00');
          public               postgres    false    227    220            M           0    0    user_chat_y2025m12    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m12 FOR VALUES FROM ('2025-12-01 00:00:00+00') TO ('2026-01-01 00:00:00+00');
          public               postgres    false    228    220            O           2604    25004    user_chat log_id    DEFAULT     t   ALTER TABLE ONLY public.user_chat ALTER COLUMN log_id SET DEFAULT nextval('public.user_chat_log_id_seq'::regclass);
 ?   ALTER TABLE public.user_chat ALTER COLUMN log_id DROP DEFAULT;
       public               postgres    false    220    219    220            N           2604    24977    users user_id    DEFAULT     n   ALTER TABLE ONLY public.users ALTER COLUMN user_id SET DEFAULT nextval('public.users_user_id_seq'::regclass);
 <   ALTER TABLE public.users ALTER COLUMN user_id DROP DEFAULT;
       public               postgres    false    218    217    218                      0    25018    user_chat_y2025m05 
   TABLE DATA           `   COPY public.user_chat_y2025m05 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    221   	K                 0    25033    user_chat_y2025m06 
   TABLE DATA           `   COPY public.user_chat_y2025m06 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    222   &K                 0    25048    user_chat_y2025m07 
   TABLE DATA           `   COPY public.user_chat_y2025m07 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    223   CK                 0    25063    user_chat_y2025m08 
   TABLE DATA           `   COPY public.user_chat_y2025m08 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    224   `K                 0    25078    user_chat_y2025m09 
   TABLE DATA           `   COPY public.user_chat_y2025m09 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    225   }K                 0    25093    user_chat_y2025m10 
   TABLE DATA           `   COPY public.user_chat_y2025m10 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    226   �K                 0    25108    user_chat_y2025m11 
   TABLE DATA           `   COPY public.user_chat_y2025m11 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    227   �K                 0    25123    user_chat_y2025m12 
   TABLE DATA           `   COPY public.user_chat_y2025m12 (log_id, sender_id, receiver_id, content, send_date) FROM stdin;
    public               postgres    false    228   �K                 0    24974    users 
   TABLE DATA           K   COPY public.users (user_id, user_name, user_name_id, password) FROM stdin;
    public               postgres    false    218   �K       #           0    0    user_chat_log_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.user_chat_log_id_seq', 1, false);
          public               postgres    false    219            $           0    0    users_user_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.users_user_id_seq', 1, true);
          public               postgres    false    217            d           2606    25007    user_chat user_chat_pkey 
   CONSTRAINT     e   ALTER TABLE ONLY public.user_chat
    ADD CONSTRAINT user_chat_pkey PRIMARY KEY (log_id, send_date);
 B   ALTER TABLE ONLY public.user_chat DROP CONSTRAINT user_chat_pkey;
       public                 postgres    false    220    220            f           2606    25024 *   user_chat_y2025m05 user_chat_y2025m05_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m05
    ADD CONSTRAINT user_chat_y2025m05_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m05 DROP CONSTRAINT user_chat_y2025m05_pkey;
       public                 postgres    false    221    4708    221    221            h           2606    25039 *   user_chat_y2025m06 user_chat_y2025m06_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m06
    ADD CONSTRAINT user_chat_y2025m06_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m06 DROP CONSTRAINT user_chat_y2025m06_pkey;
       public                 postgres    false    4708    222    222    222            j           2606    25054 *   user_chat_y2025m07 user_chat_y2025m07_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m07
    ADD CONSTRAINT user_chat_y2025m07_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m07 DROP CONSTRAINT user_chat_y2025m07_pkey;
       public                 postgres    false    223    223    223    4708            l           2606    25069 *   user_chat_y2025m08 user_chat_y2025m08_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m08
    ADD CONSTRAINT user_chat_y2025m08_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m08 DROP CONSTRAINT user_chat_y2025m08_pkey;
       public                 postgres    false    224    224    224    4708            n           2606    25084 *   user_chat_y2025m09 user_chat_y2025m09_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m09
    ADD CONSTRAINT user_chat_y2025m09_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m09 DROP CONSTRAINT user_chat_y2025m09_pkey;
       public                 postgres    false    225    225    4708    225            p           2606    25099 *   user_chat_y2025m10 user_chat_y2025m10_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m10
    ADD CONSTRAINT user_chat_y2025m10_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m10 DROP CONSTRAINT user_chat_y2025m10_pkey;
       public                 postgres    false    226    4708    226    226            r           2606    25114 *   user_chat_y2025m11 user_chat_y2025m11_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m11
    ADD CONSTRAINT user_chat_y2025m11_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m11 DROP CONSTRAINT user_chat_y2025m11_pkey;
       public                 postgres    false    227    227    227    4708            t           2606    25129 *   user_chat_y2025m12 user_chat_y2025m12_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m12
    ADD CONSTRAINT user_chat_y2025m12_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m12 DROP CONSTRAINT user_chat_y2025m12_pkey;
       public                 postgres    false    228    228    228    4708            b           2606    24981    users users_pkey 
   CONSTRAINT     S   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);
 :   ALTER TABLE ONLY public.users DROP CONSTRAINT users_pkey;
       public                 postgres    false    218            u           0    0    user_chat_y2025m05_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m05_pkey;
          public               postgres    false    221    4708    4710    4708    221    220            v           0    0    user_chat_y2025m06_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m06_pkey;
          public               postgres    false    4708    222    4712    4708    222    220            w           0    0    user_chat_y2025m07_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m07_pkey;
          public               postgres    false    223    4714    4708    4708    223    220            x           0    0    user_chat_y2025m08_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m08_pkey;
          public               postgres    false    4708    224    4716    4708    224    220            y           0    0    user_chat_y2025m09_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m09_pkey;
          public               postgres    false    225    4708    4718    4708    225    220            z           0    0    user_chat_y2025m10_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m10_pkey;
          public               postgres    false    4708    4720    226    4708    226    220            {           0    0    user_chat_y2025m11_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m11_pkey;
          public               postgres    false    4722    4708    227    4708    227    220            |           0    0    user_chat_y2025m12_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m12_pkey;
          public               postgres    false    228    4724    4708    4708    228    220            }           2606    25013 $   user_chat user_chat_receiver_id_fkey    FK CONSTRAINT     �   ALTER TABLE public.user_chat
    ADD CONSTRAINT user_chat_receiver_id_fkey FOREIGN KEY (receiver_id) REFERENCES public.users(user_id);
 I   ALTER TABLE public.user_chat DROP CONSTRAINT user_chat_receiver_id_fkey;
       public               postgres    false    4706    218    220            ~           2606    25008 "   user_chat user_chat_sender_id_fkey    FK CONSTRAINT     �   ALTER TABLE public.user_chat
    ADD CONSTRAINT user_chat_sender_id_fkey FOREIGN KEY (sender_id) REFERENCES public.users(user_id);
 G   ALTER TABLE public.user_chat DROP CONSTRAINT user_chat_sender_id_fkey;
       public               postgres    false    4706    218    220                  x������ � �            x������ � �            x������ � �            x������ � �            x������ � �            x������ � �            x������ � �            x������ � �            x�3�,I-.�47�4��b���� C�V     