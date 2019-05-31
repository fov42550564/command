//package com.whxx.sp.utils;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.*;
import java.util.regex.Pattern;


public class m {
    // 默认值
    public static <T> T ifNull(T value, T defaultValue) {
        return value == null ? defaultValue : value;
    }
    // 正则表达式检测
    public static Boolean test(String str, String reg) {
        if (null == str) {
            return false;
        }
        return Pattern.compile(reg).matcher(str).find();
    }
    // 生成一个List
    public static List<Object> list(Object... args) {
        List<Object> list = new ArrayList();
        for (Object arg : args) {
            list.add(arg);
        }
        return list;
    }
    // 生成一个Map
    public static Object object(Object... args) {
        Map context = new HashMap();
        int count  = args.length - 2;
        for (int i = 0; i <= count; i+=2) {
            context.put(args[i], args[i+1]);
        }
        return context;
    }
    // 将类转化为Map
    public static Map object(Object obj) {
        Map context = new HashMap();
        try {
            Class cls = (Class) obj.getClass();
            Field[] fields = cls.getDeclaredFields();
            for (Field field : fields) {
                field.setAccessible(true);
                context.put(field.getName(), field.get(obj));
            }
            return context;
        } catch (Exception e) {}
        return context;
    }
    // 将Map合并到Map
    public static Map merge(Map map, Object... args) {
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }
    // 将Map合并到Object
    public static Map merge(Object object, Object... args) {
        Map map = m.object(object);
        int count = args.length - 2;
        for (int i = 0; i <= count; i += 2) {
            map.put(args[i], args[i + 1]);
        }
        return map;
    }
    // 去除Map中的某些值
    public static Map omit(Map map, String... args) {
        for (String arg : args) {
            map.remove(arg);
        }
        return map;
    }
    // 提取Collections中的某一个值
    public static <T> List<T> concat(List<T> list, List<T> list1) {
        for (T item : list1) {
            list.add(item);
        }
        return list;
    }
    // 通过key值获取Object的值
    public static Object get(Object obj, String key) {
        try {
            Field field = obj.getClass().getDeclaredField(key);
            field.setAccessible(true);
            return field.get(obj);
        } catch (Exception e) {
            System.out.println(e);
        }
        return null;
    }
    // 提取Collections中的某一个值
    public static <T, M> List<T> pick(List<M> list, String key) {
        List<T> l = new ArrayList<T>();
        for (M item : list) {
            T t = (T)m.get(item, key);
            if (null != t) {
                l.add(t);
            }
        }
        return l;
    }
    // 判断字符串相等
    public static Boolean eq(String str1, String str2) {
        if (str1 == null) {
            str1 = "";
        }
        if (str2 == null) {
            str2 = "";
        }
        return str1.equals(str2);
    }
    // 获取文件扩展名
    public static String extname(String path) {
        int index = path.lastIndexOf(".");
        if (-1 == index) {
            return "";
        }
        return path.substring(index);
    }
    // 判断url是否是media
    public static Boolean isMedia(String url) {
        return test(url, "^/[a-z0-9]{24}(\\.[^/]+)?$");
    }
    //从url获取media的Id
    public static String getMediaId (String url) {
        return url.replaceAll("^/([a-z0-9]{24})(\\.[^/]+)?$", "$1");
    }
    // 从url获取meida的名称
    public static String getMediaName (String url) {
        if (null == url) {
            return null;
        }
        String extname = m.extname(url);
        String id = url.replaceAll("http://localhost:3000", "").replaceAll(extname, "");
        if (m.test(id, "^/[a-z0-9]{24}$")) {
            return id + extname;
        }
        return url;
    }
    // 通过meida的名称获取url
    public static String getMediaUrl(String name) {
        if (name == null) {
            return null;
        }
        return isMedia(name) ? "http://localhost:3000" + name : name;
    }
    // 获取默认密码
    public static String getDefaultPassWord(String ...args) {
        if (null == args[1]) {
            return args[0].substring(5);
        }
        return args[1];
    }
}
