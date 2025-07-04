PGDMP  /                     }           Chat    17.2    17.2 K    >           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                           false            ?           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                           false            @           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                           false            A           1262    24972    Chat    DATABASE     y   CREATE DATABASE "Chat" WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE_PROVIDER = libc LOCALE = 'Polish_Poland.1252';
    DROP DATABASE "Chat";
                     postgres    false            �            1259    25153    friends    TABLE     K  CREATE TABLE public.friends (
    id integer NOT NULL,
    user_1 integer NOT NULL,
    user_2 integer NOT NULL,
    accepted boolean DEFAULT false NOT NULL,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP,
    accepted_at timestamp without time zone,
    CONSTRAINT friends_check CHECK ((user_1 < user_2))
);
    DROP TABLE public.friends;
       public         heap r       postgres    false            �            1259    25152    friends_id_seq    SEQUENCE     �   CREATE SEQUENCE public.friends_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
 %   DROP SEQUENCE public.friends_id_seq;
       public               postgres    false    230            B           0    0    friends_id_seq    SEQUENCE OWNED BY     A   ALTER SEQUENCE public.friends_id_seq OWNED BY public.friends.id;
          public               postgres    false    229            �            1259    25001 	   user_chat    TABLE       CREATE TABLE public.user_chat (
    log_id integer NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
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
       public               postgres    false    220            C           0    0    user_chat_log_id_seq    SEQUENCE OWNED BY     M   ALTER SEQUENCE public.user_chat_log_id_seq OWNED BY public.user_chat.log_id;
          public               postgres    false    219            �            1259    25018    user_chat_y2025m05    TABLE     )  CREATE TABLE public.user_chat_y2025m05 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m05;
       public         heap r       postgres    false    219    220            �            1259    25033    user_chat_y2025m06    TABLE     )  CREATE TABLE public.user_chat_y2025m06 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m06;
       public         heap r       postgres    false    219    220            �            1259    25048    user_chat_y2025m07    TABLE     )  CREATE TABLE public.user_chat_y2025m07 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m07;
       public         heap r       postgres    false    219    220            �            1259    25063    user_chat_y2025m08    TABLE     )  CREATE TABLE public.user_chat_y2025m08 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m08;
       public         heap r       postgres    false    219    220            �            1259    25078    user_chat_y2025m09    TABLE     )  CREATE TABLE public.user_chat_y2025m09 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m09;
       public         heap r       postgres    false    219    220            �            1259    25093    user_chat_y2025m10    TABLE     )  CREATE TABLE public.user_chat_y2025m10 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m10;
       public         heap r       postgres    false    219    220            �            1259    25108    user_chat_y2025m11    TABLE     )  CREATE TABLE public.user_chat_y2025m11 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m11;
       public         heap r       postgres    false    219    220            �            1259    25123    user_chat_y2025m12    TABLE     )  CREATE TABLE public.user_chat_y2025m12 (
    log_id integer DEFAULT nextval('public.user_chat_log_id_seq'::regclass) NOT NULL,
    sender_id integer,
    receiver_id integer,
    content text,
    send_date timestamp with time zone DEFAULT now() NOT NULL,
    read_status boolean DEFAULT false
);
 &   DROP TABLE public.user_chat_y2025m12;
       public         heap r       postgres    false    219    220            �            1259    24974    users    TABLE     �   CREATE TABLE public.users (
    user_id integer NOT NULL,
    user_name character varying(64) NOT NULL,
    user_name_id integer NOT NULL,
    password text NOT NULL,
    email text NOT NULL
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
       public               postgres    false    218            D           0    0    users_user_id_seq    SEQUENCE OWNED BY     G   ALTER SEQUENCE public.users_user_id_seq OWNED BY public.users.user_id;
          public               postgres    false    217            K           0    0    user_chat_y2025m05    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m05 FOR VALUES FROM ('2025-05-01 00:00:00+00') TO ('2025-06-01 00:00:00+00');
          public               postgres    false    221    220            L           0    0    user_chat_y2025m06    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m06 FOR VALUES FROM ('2025-06-01 00:00:00+00') TO ('2025-07-01 00:00:00+00');
          public               postgres    false    222    220            M           0    0    user_chat_y2025m07    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m07 FOR VALUES FROM ('2025-07-01 00:00:00+00') TO ('2025-08-01 00:00:00+00');
          public               postgres    false    223    220            N           0    0    user_chat_y2025m08    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m08 FOR VALUES FROM ('2025-08-01 00:00:00+00') TO ('2025-09-01 00:00:00+00');
          public               postgres    false    224    220            O           0    0    user_chat_y2025m09    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m09 FOR VALUES FROM ('2025-09-01 00:00:00+00') TO ('2025-10-01 00:00:00+00');
          public               postgres    false    225    220            P           0    0    user_chat_y2025m10    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m10 FOR VALUES FROM ('2025-10-01 00:00:00+00') TO ('2025-11-01 00:00:00+00');
          public               postgres    false    226    220            Q           0    0    user_chat_y2025m11    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m11 FOR VALUES FROM ('2025-11-01 00:00:00+00') TO ('2025-12-01 00:00:00+00');
          public               postgres    false    227    220            R           0    0    user_chat_y2025m12    TABLE ATTACH     �   ALTER TABLE ONLY public.user_chat ATTACH PARTITION public.user_chat_y2025m12 FOR VALUES FROM ('2025-12-01 00:00:00+00') TO ('2026-01-01 00:00:00+00');
          public               postgres    false    228    220            o           2604    25156 
   friends id    DEFAULT     h   ALTER TABLE ONLY public.friends ALTER COLUMN id SET DEFAULT nextval('public.friends_id_seq'::regclass);
 9   ALTER TABLE public.friends ALTER COLUMN id DROP DEFAULT;
       public               postgres    false    229    230    230            T           2604    25004    user_chat log_id    DEFAULT     t   ALTER TABLE ONLY public.user_chat ALTER COLUMN log_id SET DEFAULT nextval('public.user_chat_log_id_seq'::regclass);
 ?   ALTER TABLE public.user_chat ALTER COLUMN log_id DROP DEFAULT;
       public               postgres    false    219    220    220            S           2604    24977    users user_id    DEFAULT     n   ALTER TABLE ONLY public.users ALTER COLUMN user_id SET DEFAULT nextval('public.users_user_id_seq'::regclass);
 <   ALTER TABLE public.users ALTER COLUMN user_id DROP DEFAULT;
       public               postgres    false    217    218    218            ;          0    25153    friends 
   TABLE DATA           X   COPY public.friends (id, user_1, user_2, accepted, created_at, accepted_at) FROM stdin;
    public               postgres    false    230   p`       2          0    25018    user_chat_y2025m05 
   TABLE DATA           m   COPY public.user_chat_y2025m05 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    221   �`       3          0    25033    user_chat_y2025m06 
   TABLE DATA           m   COPY public.user_chat_y2025m06 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    222   d       4          0    25048    user_chat_y2025m07 
   TABLE DATA           m   COPY public.user_chat_y2025m07 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    223   -d       5          0    25063    user_chat_y2025m08 
   TABLE DATA           m   COPY public.user_chat_y2025m08 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    224   Jd       6          0    25078    user_chat_y2025m09 
   TABLE DATA           m   COPY public.user_chat_y2025m09 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    225   gd       7          0    25093    user_chat_y2025m10 
   TABLE DATA           m   COPY public.user_chat_y2025m10 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    226   �d       8          0    25108    user_chat_y2025m11 
   TABLE DATA           m   COPY public.user_chat_y2025m11 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    227   �d       9          0    25123    user_chat_y2025m12 
   TABLE DATA           m   COPY public.user_chat_y2025m12 (log_id, sender_id, receiver_id, content, send_date, read_status) FROM stdin;
    public               postgres    false    228   �d       0          0    24974    users 
   TABLE DATA           R   COPY public.users (user_id, user_name, user_name_id, password, email) FROM stdin;
    public               postgres    false    218   �d       E           0    0    friends_id_seq    SEQUENCE SET     <   SELECT pg_catalog.setval('public.friends_id_seq', 4, true);
          public               postgres    false    229            F           0    0    user_chat_log_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.user_chat_log_id_seq', 46, true);
          public               postgres    false    219            G           0    0    users_user_id_seq    SEQUENCE SET     ?   SELECT pg_catalog.setval('public.users_user_id_seq', 5, true);
          public               postgres    false    217            �           2606    25161    friends friends_pkey 
   CONSTRAINT     R   ALTER TABLE ONLY public.friends
    ADD CONSTRAINT friends_pkey PRIMARY KEY (id);
 >   ALTER TABLE ONLY public.friends DROP CONSTRAINT friends_pkey;
       public                 postgres    false    230            �           2606    25163 !   friends friends_user_1_user_2_key 
   CONSTRAINT     f   ALTER TABLE ONLY public.friends
    ADD CONSTRAINT friends_user_1_user_2_key UNIQUE (user_1, user_2);
 K   ALTER TABLE ONLY public.friends DROP CONSTRAINT friends_user_1_user_2_key;
       public                 postgres    false    230    230            x           2606    25007    user_chat user_chat_pkey 
   CONSTRAINT     e   ALTER TABLE ONLY public.user_chat
    ADD CONSTRAINT user_chat_pkey PRIMARY KEY (log_id, send_date);
 B   ALTER TABLE ONLY public.user_chat DROP CONSTRAINT user_chat_pkey;
       public                 postgres    false    220    220            z           2606    25024 *   user_chat_y2025m05 user_chat_y2025m05_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m05
    ADD CONSTRAINT user_chat_y2025m05_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m05 DROP CONSTRAINT user_chat_y2025m05_pkey;
       public                 postgres    false    221    221    221    4728            |           2606    25039 *   user_chat_y2025m06 user_chat_y2025m06_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m06
    ADD CONSTRAINT user_chat_y2025m06_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m06 DROP CONSTRAINT user_chat_y2025m06_pkey;
       public                 postgres    false    222    222    222    4728            ~           2606    25054 *   user_chat_y2025m07 user_chat_y2025m07_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m07
    ADD CONSTRAINT user_chat_y2025m07_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m07 DROP CONSTRAINT user_chat_y2025m07_pkey;
       public                 postgres    false    223    223    4728    223            �           2606    25069 *   user_chat_y2025m08 user_chat_y2025m08_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m08
    ADD CONSTRAINT user_chat_y2025m08_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m08 DROP CONSTRAINT user_chat_y2025m08_pkey;
       public                 postgres    false    224    224    4728    224            �           2606    25084 *   user_chat_y2025m09 user_chat_y2025m09_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m09
    ADD CONSTRAINT user_chat_y2025m09_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m09 DROP CONSTRAINT user_chat_y2025m09_pkey;
       public                 postgres    false    225    4728    225    225            �           2606    25099 *   user_chat_y2025m10 user_chat_y2025m10_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m10
    ADD CONSTRAINT user_chat_y2025m10_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m10 DROP CONSTRAINT user_chat_y2025m10_pkey;
       public                 postgres    false    226    4728    226    226            �           2606    25114 *   user_chat_y2025m11 user_chat_y2025m11_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m11
    ADD CONSTRAINT user_chat_y2025m11_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m11 DROP CONSTRAINT user_chat_y2025m11_pkey;
       public                 postgres    false    227    4728    227    227            �           2606    25129 *   user_chat_y2025m12 user_chat_y2025m12_pkey 
   CONSTRAINT     w   ALTER TABLE ONLY public.user_chat_y2025m12
    ADD CONSTRAINT user_chat_y2025m12_pkey PRIMARY KEY (log_id, send_date);
 T   ALTER TABLE ONLY public.user_chat_y2025m12 DROP CONSTRAINT user_chat_y2025m12_pkey;
       public                 postgres    false    4728    228    228    228            t           2606    25139    users users_email_unique 
   CONSTRAINT     T   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_email_unique UNIQUE (email);
 B   ALTER TABLE ONLY public.users DROP CONSTRAINT users_email_unique;
       public                 postgres    false    218            v           2606    24981    users users_pkey 
   CONSTRAINT     S   ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (user_id);
 :   ALTER TABLE ONLY public.users DROP CONSTRAINT users_pkey;
       public                 postgres    false    218            �           1259    25176    idx_friends_accepted    INDEX     L   CREATE INDEX idx_friends_accepted ON public.friends USING btree (accepted);
 (   DROP INDEX public.idx_friends_accepted;
       public                 postgres    false    230            �           1259    25174    idx_friends_user_1    INDEX     H   CREATE INDEX idx_friends_user_1 ON public.friends USING btree (user_1);
 &   DROP INDEX public.idx_friends_user_1;
       public                 postgres    false    230            �           1259    25177    idx_friends_user_1_accepted    INDEX     [   CREATE INDEX idx_friends_user_1_accepted ON public.friends USING btree (user_1, accepted);
 /   DROP INDEX public.idx_friends_user_1_accepted;
       public                 postgres    false    230    230            �           1259    25175    idx_friends_user_2    INDEX     H   CREATE INDEX idx_friends_user_2 ON public.friends USING btree (user_2);
 &   DROP INDEX public.idx_friends_user_2;
       public                 postgres    false    230            �           1259    25178    idx_friends_user_2_accepted    INDEX     [   CREATE INDEX idx_friends_user_2_accepted ON public.friends USING btree (user_2, accepted);
 /   DROP INDEX public.idx_friends_user_2_accepted;
       public                 postgres    false    230    230            �           0    0    user_chat_y2025m05_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m05_pkey;
          public               postgres    false    4730    221    4728    4728    221    220            �           0    0    user_chat_y2025m06_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m06_pkey;
          public               postgres    false    4732    4728    222    4728    222    220            �           0    0    user_chat_y2025m07_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m07_pkey;
          public               postgres    false    4734    4728    223    4728    223    220            �           0    0    user_chat_y2025m08_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m08_pkey;
          public               postgres    false    224    4736    4728    4728    224    220            �           0    0    user_chat_y2025m09_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m09_pkey;
          public               postgres    false    4738    4728    225    4728    225    220            �           0    0    user_chat_y2025m10_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m10_pkey;
          public               postgres    false    4740    4728    226    4728    226    220            �           0    0    user_chat_y2025m11_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m11_pkey;
          public               postgres    false    4728    4742    227    4728    227    220            �           0    0    user_chat_y2025m12_pkey    INDEX ATTACH     S   ALTER INDEX public.user_chat_pkey ATTACH PARTITION public.user_chat_y2025m12_pkey;
          public               postgres    false    4744    228    4728    4728    228    220            �           2606    25164    friends friends_user_1_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.friends
    ADD CONSTRAINT friends_user_1_fkey FOREIGN KEY (user_1) REFERENCES public.users(user_id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.friends DROP CONSTRAINT friends_user_1_fkey;
       public               postgres    false    4726    230    218            �           2606    25169    friends friends_user_2_fkey    FK CONSTRAINT     �   ALTER TABLE ONLY public.friends
    ADD CONSTRAINT friends_user_2_fkey FOREIGN KEY (user_2) REFERENCES public.users(user_id) ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.friends DROP CONSTRAINT friends_user_2_fkey;
       public               postgres    false    4726    230    218            �           2606    25013 $   user_chat user_chat_receiver_id_fkey    FK CONSTRAINT     �   ALTER TABLE public.user_chat
    ADD CONSTRAINT user_chat_receiver_id_fkey FOREIGN KEY (receiver_id) REFERENCES public.users(user_id);
 I   ALTER TABLE public.user_chat DROP CONSTRAINT user_chat_receiver_id_fkey;
       public               postgres    false    4726    220    218            �           2606    25008 "   user_chat user_chat_sender_id_fkey    FK CONSTRAINT     �   ALTER TABLE public.user_chat
    ADD CONSTRAINT user_chat_sender_id_fkey FOREIGN KEY (sender_id) REFERENCES public.users(user_id);
 G   ALTER TABLE public.user_chat DROP CONSTRAINT user_chat_sender_id_fkey;
       public               postgres    false    220    218    4726            ;   F   x�M���0�7T��p�9�"� e���?��!��[�9�#�Q���Lʾ��T�t�k5i\��[�T�K�      2   :  x�ݖ͎�F���S��fBr8_�H{(��r�E���vm��䴻O_Η���y���?r8$���7��oȼ�C���Q^�E�@�e�!��0/���Vl ��0CcO��&�t�FP���XhjP\���r~�?N��x�����_��r�`[�Ƞ3V��D�'�h0o�cV-����9���0�硟�w�@�"K&�,;�����߸<lӅA1�ʺ�&x�+��3M����EɬU@�Y6lY�+�xI���C_X������<�;<L��t�vdT@��^���lq͊<�P�̥ZU:6(����s�q8��k�b[h�Д�h#G??��p����[o]6�بxm�m��$�C-Wo����0�����`;�9�8x]��d����R4��G�m�ƛh�-���%����2��̙�}O�Vk��k�x�x-'�x��P�;�WظR���8lI&e<���<M���,�a)�2�wReH� �%���}�%Xp���\���
5�KT���cI�@�\��5�V�5.M�-����/����b�Fp�N��|��~��6��̾*�\�O�~Z�!N0re���S�F��g5-���e��"%�W�LZ3[�}k'�&x�ĥM���uE �C���"���M[�Y�����*�8�
d
pѷ|8H�l4��Z��sg�vo����y�:�]�_۩N�m���í��R�s�%+kY2O��e�p���˲��D�@*���8W�÷~<��v?�N��0wmZ�?������_��oy�C���[�Pb ��$������m\���HF�0�HZ�����v�e�%�      3      x������ � �      4      x������ � �      5      x������ � �      6      x������ � �      7      x������ � �      8      x������ � �      9      x������ � �      0   g   x�3�,I-.�47�4��@�CA�^j)�1�SQbUf����gFbqN>T�/��!5713G/9?�˄3�ɘ�����b�1�S��dNs#s3�,D�W� ��(f     