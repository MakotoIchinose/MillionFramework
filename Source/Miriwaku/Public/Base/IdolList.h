// Copyright (C) TheHoodieGuy02

#pragma once

#include "CoreMinimal.h"

/**
 * Enum of idol names.
 */
UENUM()
enum class EIdolNames : uint8
{
 // Filler None element. We want to start counting from 1.
 None      UMETA(DisplayName = "None"),
 
 // OG All-Stars line-up
 
 Haruka    UMETA(DisplayName = "Haruka Amami"),
 Chihaya   UMETA(DisplayName = "Chihaya Kisaragi"),
 Miki      UMETA(DisplayName = "Miki Hoshii"),
 Yukiho    UMETA(DisplayName = "Yukiho Hagiwara"),
 Yayoi     UMETA(DisplayName = "Yayoi Takatsuki"),
 Makoto    UMETA(DisplayName = "Makoto Kikuchi"),
 Iori      UMETA(DisplayName = "Iori Minase"),
 Takane    UMETA(DisplayName = "Takane Shijou"),
 Ritsuko   UMETA(DisplayName = "Ritsuko Akizuki"),
 Azusa     UMETA(DisplayName = "Azusa Miura"),
 Ami       UMETA(DisplayName = "Ami Futami"),
 Mami      UMETA(DisplayName = "Mami Futami"),
 Hibiki    UMETA(DisplayName = "Hibiki Ganaha"),
 
 // Sankyuu line-up
 
 Mirai     UMETA(DisplayName = "Mirai Kasuga"),
 Shizuka   UMETA(DisplayName = "Shizuka Mogami"),
 Tsubasa   UMETA(DisplayName = "Tsubasa Ibuki"),
 Kotoha    UMETA(DisplayName = "Kotoha Tanaka"),
 Elena     UMETA(DisplayName = "Elena Shimabara"),
 Minako    UMETA(DisplayName = "Minako Satake"),
 Megumi    UMETA(DisplayName = "Megumi Tokoro"),
 Matsuri   UMETA(DisplayName = "Matsuri Tokugawa"),
 Serika    UMETA(DisplayName = "Serika Hakozaki"),
 Akane     UMETA(DisplayName = "Akane Nonohara"),
 Roco      UMETA(DisplayName = "Roco Handa"),
 Yuriko    UMETA(DisplayName = "Yuriko Nanao"),
 Sayoko    UMETA(DisplayName = "Sayoko Takayama"),
 Arisa     UMETA(DisplayName = "Arisa Matsuda"),
 Umi       UMETA(DisplayName = "Umi Kousaka"),
 Iku       UMETA(DisplayName = "Iku Nakatani"),
 Tomoka    UMETA(DisplayName = "Tomoka Tenkubashi"),
 Emily     UMETA(DisplayName = "Emily Stewart"),
 Shiho     UMETA(DisplayName = "Shiho Kitazawa"),
 Ayumu     UMETA(DisplayName = "Ayumu Maihama"),
 Hinata    UMETA(DisplayName = "Hinata Kinoshita"),
 Kana      UMETA(DisplayName = "Kana Yabuki"),
 Nao       UMETA(DisplayName = "Nao Yokoyama"),
 Chizuru   UMETA(DisplayName = "Chizuru Nikaido"),
 Konomi    UMETA(DisplayName = "Konomi Baba"),
 Tamaki    UMETA(DisplayName = "Tamaki Ogami"),
 Fuka      UMETA(DisplayName = "Fuka Toyokawa"),
 Miya      UMETA(DisplayName = "Miya Miyao"),
 Noriko    UMETA(DisplayName = "Noriko Fukuda"),
 Mizuki    UMETA(DisplayName = "Mizuki Makabe"),
 Karen     UMETA(DisplayName = "Karen Shinomiya"),
 Rio       UMETA(DisplayName = "Rio Momose"),
 Subaru    UMETA(DisplayName = "Subaru Nagayoshi"),
 Reika     UMETA(DisplayName = "Reika Kitakami"),
 Momoko    UMETA(DisplayName = "Momoko Suou"),
 Julia     UMETA(DisplayName = "Julia"),
 Tsumugi   UMETA(DisplayName = "Tsumugi Shiraishi"),
 Kaori     UMETA(DisplayName = "Kaori Sakuramori"),

 // 765PRO staffs
 
 Kotori    UMETA(DisplayName = "Kotori Otonashi"),
 Nanto     UMETA(DisplayName = "Aoba Misaki"),

 // 961PRO guests

 Leon      UMETA(DisplayName = "Leon"),
 Shika     UMETA(DisplayName = "Shika"),
 
 // 346PRO guests
 
 Shiki     UMETA(DisplayName = "Shiki Ichinose"),
 Frederica UMETA(DisplayName = "Frederica Miyamoto")
};


